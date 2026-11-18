// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include "fifechan/widgets/speechbubble.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <vector>

#include "fifechan/graphics.hpp"
#include "fifechan/math.hpp"
#include "fifechan/point.hpp"
#include "fifechan/widget.hpp"

namespace fcn
{

    namespace
    {

        using PointV = std::vector<Point>;

        // TODO: this PI is actually PI_F or from math: FLT_PI
        constexpr float PI = std::numbers::pi_v<float>;

        // ── Arc helper ───────────────────────────────────────────────────
        int arcSegments(int r, float span)
        {
            float const absSpan = std::abs(span);
            // chord error < 0.25 px:  n = ceil(span / (2 * acos(1 - 0.25/r)))
            // Approximate formula: sqrt(2 * r * 0.25) per radian
            int const byRadius = std::max(1, static_cast<int>(std::ceil(0.35F * static_cast<float>(r))));
            int const byAngle  = std::max(1, static_cast<int>(std::ceil(absSpan / (PI / 12.0F)))); // 15° per segment
            return std::max({4, byRadius, byAngle});
        }

        void addArc(PointV& pts, int cx, int cy, int r, float startRad, float endRad)
        {
            if (r <= 0) {
                return;
            }
            float const span = endRad - startRad;
            int const n      = arcSegments(r, span);
            for (int i = 1; i <= n; ++i) {
                float const t = static_cast<float>(i) / static_cast<float>(n);
                float const a = startRad + (span * t);
                pts.emplace_back(
                    cx + static_cast<int>(static_cast<float>(r) * std::cos(a)),
                    cy + static_cast<int>(static_cast<float>(r) * std::sin(a)));
            }
        }

        // ── Comic body outlines ─────────────────────────────────────────
        void addEllipseOutline(PointV& pts, int cx, int cy, int rx, int ry, float aStart, float aEnd, int n)
        {
            if (rx <= 0 || ry <= 0 || n < 3) {
                return;
            }
            for (int i = 0; i < n; ++i) {
                float const t = aStart + ((aEnd - aStart) * static_cast<float>(i) / static_cast<float>(n - 1));
                pts.emplace_back(
                    cx + static_cast<int>(static_cast<float>(rx) * std::cos(t)),
                    cy + static_cast<int>(static_cast<float>(ry) * std::sin(t)));
            }
        }

        void addCloudOutline(PointV& pts, int cx, int cy, int rx, int ry, int n)
        {
            float const Rx = static_cast<float>(rx) * 0.92F;
            float const Ry = static_cast<float>(ry) * 0.92F;
            for (int i = 0; i < n; ++i) {
                float const t   = 2.0F * PI * static_cast<float>(i) / static_cast<float>(n);
                float const c   = std::cos(t);
                float const s   = std::sin(t);
                float const inv = ((c * c) / (Rx * Rx)) + ((s * s) / (Ry * Ry));
                float const rho = 1.0F / std::sqrt(inv);
                float const r   = rho * (1.0F + (0.14F * std::cos((3.0F * t) + 0.6F)));
                pts.emplace_back(cx + static_cast<int>(r * c), cy + static_cast<int>(r * s));
            }
        }

        void addBurstOutline(PointV& pts, int cx, int cy, int bodyW, int bodyH, int spikes)
        {
            float const Rx = static_cast<float>(bodyW) * 0.5F;
            float const Ry = static_cast<float>(bodyH) * 0.5F;
            int const n    = std::max(4, spikes * 2);
            for (int i = 0; i < n; ++i) {
                float const t   = 2.0F * PI * static_cast<float>(i) / static_cast<float>(n);
                float const c   = std::cos(t);
                float const s   = std::sin(t);
                float const inv = ((c * c) / (Rx * Rx)) + ((s * s) / (Ry * Ry));
                float const rho = 1.0F / std::sqrt(inv);
                float const r   = rho * ((i % 2 == 0) ? 1.0F : 0.60F);
                pts.emplace_back(cx + static_cast<int>(r * c), cy + static_cast<int>(r * s));
            }
        }

        // ── Corner tail (classic comic tail at a rounded corner) ────────
        // Replaces the middle of a corner arc (startDeg→endDeg, degrees) with
        // a triangle that protrudes outward along the corner bisector.
        void addCornerTail(PointV& pts, int ccx, int ccy, int r, int tw, int th, float startDeg, float endDeg)
        {
            float const start = startDeg * (PI / 180.0F);
            float const end   = endDeg * PI / 180.0F;
            float const span  = end - start;
            float const bis   = start + (0.5F * span);

            float alpha = 0.5F * span;
            if (r > 0 && tw > 0) {
                float const sinv = std::min(static_cast<float>(tw) / static_cast<float>(r), 1.0F);
                alpha            = std::clamp((0.5F * span) - std::asin(sinv), 0.0F, (0.5F * span));
            }

            addArc(pts, ccx, ccy, r, start, start + alpha);

            // The body reserves `th` on each adjacent side, so the tail tip may
            // reach all the way to the widget's corner: distance = (r+th)/cos(45°).
            float const tipDist = std::numbers::sqrt2_v<float> * static_cast<float>(r + th);
            pts.emplace_back(
                ccx + static_cast<int>(tipDist * std::cos(bis)), ccy + static_cast<int>(tipDist * std::sin(bis)));

            float const a2 = end - alpha;
            pts.emplace_back(
                ccx + static_cast<int>(static_cast<float>(r) * std::cos(a2)),
                ccy + static_cast<int>(static_cast<float>(r) * std::sin(a2)));

            addArc(pts, ccx, ccy, r, a2, end);
        }

        // ── Spine-based tail generation ─────────────────────────────────

        struct TailSpinePt
        {
                float x, y;   // position (relative to attachment centre)
                float nx, ny; // left-pointing unit normal
        };

        TailSpinePt* buildTailSpine(
            float height, float curvature, float hook, int segments, TailSpinePt* out, float yOffset = 0.0F)
        {
            for (int i = 0; i <= segments; ++i) {
                float const t       = static_cast<float>(i) / static_cast<float>(segments);
                float const curvOff = curvature * std::sin(t * PI) * height * 0.25F;
                float const hookOff = hook * t * t * height * 0.3F;
                out[i].x            = curvOff + hookOff;
                out[i].y            = yOffset + (t * height);
            }
            for (int i = 0; i <= segments; ++i) {
                float dx = 0.0F;
                float dy = 0.0F;
                if (i == 0) {
                    dx = out[1].x - out[0].x;
                    dy = out[1].y - out[0].y;
                } else if (i == segments) {
                    dx = out[i].x - out[i - 1].x;
                    dy = out[i].y - out[i - 1].y;
                } else {
                    dx = out[i + 1].x - out[i - 1].x;
                    dy = out[i + 1].y - out[i - 1].y;
                }
                float const len = std::sqrt((dx * dx) + (dy * dy));
                if (len > 0.0001F) {
                    dx /= len;
                    dy /= len;
                }
                out[i].nx = -dy;
                out[i].ny = dx;
            }
            return out + (segments + 1);
        }

        float tailHalfWidth(float t, int tw, SpeechBubble::TailProfile const & p)
        {
            float const base = static_cast<float>(tw);
            float const tip  = base * p.tipWidthRatio;
            float w          = tip + ((base - tip) * std::pow(1.0F - t, p.taperExponent));
            if (p.jaggedness > 0.0F && p.teeth > 0) {
                w += p.jaggedness * base * std::sin(t * static_cast<float>(p.teeth) * 2.0F * PI) * (1.0F - (t * 0.5F));
            }
            return std::max(0.0F, w);
        }

        void rotatePoint(float& x, float& y, int dir)
        {
            // The canonical tail spine runs along +y (forward progression).
            // The outbound edge is at +x (right side of forward), the
            // inbound edge at -x (left side of forward).  We rotate so
            // that for each direction the forward (+y) points away from
            // the body in widget space.
            //
            //   dir  name      mapping          canonical +y → widget
            //   0    Down      (x,y) → (x,y)    (0,+h) → (0,+h) ↓
            //   1    Up        (x,y) → (x,-y)   (0,+h) → (0,-h) ↑
            //   2    Left      (x,y) → (-y,x)   (0,+h) → (-h,0) ←
            //   3    Right     (x,y) → (y,-x)   (0,+h) → (+h,0) →
            switch (dir) {
            case 0:
                break; // Down  (identity)       — canonical +y → widget +y
            case 1:
                y = -y;
                break; // Up    (negate Y)       — canonical +y → widget -y
            case 2: {
                float const t = x;
                x             = -y;
                y             = t;
                break;
            } // Left  ( 90° CCW)               — canonical +y → widget -x
            case 3: {
                float const t = x;
                x             = y;
                y             = -t;
                break;
            } // Right ( 90° CW)                — canonical +y → widget +x
            default:
                break;
            }
        }

        void addSpineTail(
            PointV& pts,
            int cx,
            int cy,
            SpeechBubble::TailProfile const & p,
            int tw,
            int th,
            int dir,
            int rightArcEndX = -999999,
            int leftArcEndX  = -999999)
        {
            int const n = std::max(1, p.segments);
            std::vector<TailSpinePt> spine(static_cast<std::size_t>(n + 1));

            int const jf       = std::min(tw, std::max(1, std::min({12, tw / 3, tw - 1})));
            bool const doProng = (p.prongDepth > 0.0F && n >= 2);

            // A rounded/blunt tip cap hangs beyond the spine tip; shrink the
            // spine so the cap's outer edge still lands within the tail height
            // budget (th) instead of spilling past it.
            float const capInset = (!doProng) ? tailHalfWidth(1.0F, tw, p) : 0.0F;
            int const spineH     = std::max(1, th - jf - static_cast<int>(capInset));
            buildTailSpine(static_cast<float>(spineH), p.curvature, p.hook, n, spine.data(), static_cast<float>(jf));

            // ── Right transition (body arc end → tail outbound) ──
            // When arc endpoints are provided, use a cubic Bezier fillet
            // from the corner-arc endpoint to the tail outbound entry,
            // producing a smooth curve that replaces the flat shoulder.
            bool const connectArcs = (rightArcEndX > -999990);
            if (connectArcs) {
                float const p0x = static_cast<float>(rightArcEndX - cx);
                float const p3x = static_cast<float>(tw);
                float const p3y = static_cast<float>(jf);
                // P1 = P0 (LEFT tangent at arc end)
                // P2 = (tw+jf, 0) so P2→P3 gives LEFT+DOWN tangent
                float const p2x = p3x + std::min(p3y, std::abs(p3x - p0x));
                for (int bt = 0; bt <= 10; ++bt) {
                    float const t = static_cast<float>(bt) / 10.0F;
                    float const u = 1.0F - t;
                    float fx =
                        (u * u * u * p0x) + (3.0F * u * u * t * p0x) + (3.0F * u * t * t * p2x) + (t * t * t * p3x);
                    float fy = t * t * t * p3y;
                    rotatePoint(fx, fy, dir);
                    pts.emplace_back(cx + static_cast<int>(fx), cy + static_cast<int>(fy));
                }
            } else {
                // ── Right junction fillet (body edge → tail outbound) ──
                for (int bt = 0; bt <= 10; ++bt) {
                    float const t  = static_cast<float>(bt) / 10.0F;
                    float const u  = 1.0F - t;
                    float const sx = (u * u * u * static_cast<float>(tw - jf)) +
                                     (3.0F * u * u * t * static_cast<float>(tw - jf)) +
                                     (3.0F * u * t * t * static_cast<float>(tw)) + (t * t * t * static_cast<float>(tw));
                    float const sy = t * t * t * static_cast<float>(jf);
                    float fx       = sx;
                    float fy       = sy;
                    rotatePoint(fx, fy, dir);
                    pts.emplace_back(cx + static_cast<int>(fx), cy + static_cast<int>(fy));
                }
            }

            // Outbound edge: from entry junction toward tip
            {
                // Wall endpoint at the tip, used as the reference for the mouth
                // blend (straight junction→tip line).
                float const tipHw       = tailHalfWidth(1.0F, tw, p);
                std::size_t const n_idx = static_cast<std::size_t>(n);
                float obEx              = spine[n_idx].x - (spine[n_idx].nx * tipHw);
                float obEy              = spine[n_idx].y - (spine[n_idx].ny * tipHw);
                if (spine[n_idx].ny > 0.0F) {
                    obEx = spine[n_idx].x + tipHw;
                    obEy = spine[n_idx].y;
                }
                obEy              = std::max(obEy, 0.0F);
                float const bendW = std::max(2.0F, static_cast<float>(n) * 0.5F);
                for (int i = 0; i <= n; ++i) {
                    float const t  = static_cast<float>(i) / static_cast<float>(n);
                    float const hw = tailHalfWidth(t, tw, p);
                    float lx       = 0.0F;
                    float ly       = 0.0F;
                    if (i == 0) {
                        // Fixed junction point matching the fillet endpoint so the
                        // tail is always flush with the body seam.
                        lx = static_cast<float>(tw);
                        ly = static_cast<float>(jf);
                    } else {
                        std::size_t const i_idx = static_cast<std::size_t>(i);
                        lx                      = spine[i_idx].x - (spine[i_idx].nx * hw);
                        ly                      = spine[i_idx].y - (spine[i_idx].ny * hw);
                        // When the spine bends rightward its edge normal tilts up and
                        // would fold the edge back over the body (or over itself).
                        // Keep the width horizontal for those segments so the edge
                        // always runs away from the body.
                        if (spine[i_idx].ny > 0.0F) {
                            lx = spine[i_idx].x + hw;
                            ly = spine[i_idx].y;
                        }
                        // Blend the wall points near the mouth toward the straight
                        // junction→tip line so the mouth opens smoothly from (±tw, jf)
                        // instead of spiking off the spine offset normal.
                        float const bl = std::clamp((static_cast<float>(i) - 1.0F) / bendW, 0.0F, 1.0F);
                        float const pr = static_cast<float>(i) / static_cast<float>(n);
                        lx = ((static_cast<float>(tw) + ((obEx - static_cast<float>(tw)) * pr)) * (1.0F - bl)) +
                             (lx * bl);
                        ly = ((static_cast<float>(jf) + ((obEy - static_cast<float>(jf)) * pr)) * (1.0F - bl)) +
                             (ly * bl);
                    }
                    ly = std::max(ly, 0.0F); // clamp: keep the tail below the body seam
                    if (doProng && i >= n - 2) {
                        float const prT = static_cast<float>(i - (n - 2)) * 0.5F;
                        lx += p.prongDepth * (1.0F - prT) * static_cast<float>(tw);
                        ly += p.prongDepth * prT * static_cast<float>(th) * 0.3F;
                    }

                    rotatePoint(lx, ly, dir);

                    pts.emplace_back(cx + static_cast<int>(lx), cy + static_cast<int>(ly));
                }
            }

            // Pronged: notch bridge between prongs
            if (doProng) {
                float bx2 = 0.0F;
                float by2 = 0.0F;
                {
                    float const hw          = tailHalfWidth(1.0F, tw, p);
                    std::size_t const n_idx = static_cast<std::size_t>(n);
                    float const snx         = spine[n_idx].x;
                    float const sny         = spine[n_idx].y;
                    float lpx               = snx + (spine[n_idx].nx * hw) - (p.prongDepth * static_cast<float>(tw));
                    float lpy = sny + (spine[n_idx].ny * hw) + (p.prongDepth * static_cast<float>(th) * 0.3F);
                    rotatePoint(lpx, lpy, dir);
                    bx2 = lpx;
                    by2 = lpy;
                }
                pts.emplace_back(cx + static_cast<int>(bx2), cy + static_cast<int>(by2));
            }

            // ── Tip cap (rounded tip for non-pronged tails) ──
            if (!doProng) {
                float const tipHW = tailHalfWidth(1.0F, tw, p);
                if (tipHW > 0.5F) {
                    std::size_t const n_idx = static_cast<std::size_t>(n);
                    float const capCX       = spine[n_idx].x;
                    float const capCY       = spine[n_idx].y;
                    // outbound direction from spine tip centre
                    float const sa = std::atan2(-spine[n_idx].ny, -spine[n_idx].nx);
                    // inbound direction from spine tip centre
                    float const ea = std::atan2(spine[n_idx].ny, spine[n_idx].nx);
                    // Traverse the cap from the outbound corner to the inbound
                    // corner keeping the polygon winding consistent so the arc
                    // bulges outward instead of notching inward.
                    float span = ea - sa;
                    if (span < 0) {
                        span += 2.0F * PI;
                    }
                    int const ncap = arcSegments(static_cast<int>(tipHW), span);
                    for (int i = 1; i <= ncap; ++i) {
                        float const t = static_cast<float>(i) / static_cast<float>(ncap);
                        float const a = sa + (span * t);
                        float px      = capCX + (tipHW * std::cos(a));
                        float py      = capCY + (tipHW * std::sin(a));
                        rotatePoint(px, py, dir);
                        pts.emplace_back(cx + static_cast<int>(px), cy + static_cast<int>(py));
                    }
                }
            }

            // Inbound edge: from tip back toward exit junction
            {
                float const tipHw       = tailHalfWidth(1.0F, tw, p);
                std::size_t const n_idx = static_cast<std::size_t>(n);
                float ibEx              = spine[n_idx].x + (spine[n_idx].nx * tipHw);
                float ibEy              = spine[n_idx].y + (spine[n_idx].ny * tipHw);
                if (spine[n_idx].ny > 0.0F) {
                    ibEx = spine[n_idx].x - tipHw;
                    ibEy = spine[n_idx].y;
                }
                ibEy              = std::max(ibEy, 0.0F);
                float const bendW = std::max(2.0F, static_cast<float>(n) * 0.5F);
                for (int i = n; i >= 0; --i) {
                    float const t  = static_cast<float>(i) / static_cast<float>(n);
                    float const hw = tailHalfWidth(t, tw, p);
                    float rx       = 0.0F;
                    float ry       = 0.0F;
                    if (i == 0) {
                        // Fixed junction point matching the left fillet endpoint
                        rx = static_cast<float>(-tw);
                        ry = static_cast<float>(jf);
                    } else {
                        std::size_t const i_idx = static_cast<std::size_t>(i);
                        rx                      = spine[i_idx].x + (spine[i_idx].nx * hw);
                        ry                      = spine[i_idx].y + (spine[i_idx].ny * hw);
                        if (spine[i_idx].ny > 0.0F) {
                            rx = spine[i_idx].x - hw;
                            ry = spine[i_idx].y;
                        }
                        // Mouth blend: keep the wall near the straight
                        // junction→tip line until the curve takes over.
                        float const bl = std::clamp((static_cast<float>(i) - 1.0F) / bendW, 0.0F, 1.0F);
                        float const pr = static_cast<float>(i) / static_cast<float>(n);
                        rx = ((static_cast<float>(-tw) + ((ibEx + static_cast<float>(tw)) * pr)) * (1.0F - bl)) +
                             (rx * bl);
                        ry = ((static_cast<float>(jf) + ((ibEy - static_cast<float>(jf)) * pr)) * (1.0F - bl)) +
                             (ry * bl);
                    }
                    ry = std::max(ry, 0.0F); // clamp: keep the tail below the body seam
                    if (doProng && i >= n - 2) {
                        float const prT = static_cast<float>(i - (n - 2)) * 0.5F;
                        rx -= p.prongDepth * (1.0F - prT) * static_cast<float>(tw);
                        ry += p.prongDepth * prT * static_cast<float>(th) * 0.3F;
                    }

                    rotatePoint(rx, ry, dir);

                    pts.emplace_back(cx + static_cast<int>(rx), cy + static_cast<int>(ry));
                }
            }

            // ── Left transition (tail inbound → body arc start) ──
            // Cubic Bezier from the tail inbound end (-tw, jf) to the
            // corner-arc start (leftArcEndX-cx, 0), smoothly curving into
            // the arc's LEFT tangent.
            if (connectArcs) {
                float const p0x = static_cast<float>(-tw);
                float const p0y = static_cast<float>(jf);
                float const p3x = static_cast<float>(leftArcEndX - cx);
                // P1 = (p0x, 0) — UP tangent at tail inbound end
                // P2 = (p3x+jf, 0) — to RIGHT of P3 so P2→P3 = LEFT
                float const p2x = p3x + std::min(p0y, std::abs(p3x - p0x));
                for (int bt = 0; bt <= 10; ++bt) {
                    float const t = static_cast<float>(bt) / 10.0F;
                    float const u = 1.0F - t;
                    float fx =
                        (u * u * u * p0x) + (3.0F * u * u * t * p0x) + (3.0F * u * t * t * p2x) + (t * t * t * p3x);
                    float fy = u * u * u * p0y;
                    rotatePoint(fx, fy, dir);
                    pts.emplace_back(cx + static_cast<int>(fx), cy + static_cast<int>(fy));
                }
            } else {
                // ── Left junction fillet (tail inbound → body edge) ──
                for (int bt = 0; bt <= 10; ++bt) {
                    float const t = static_cast<float>(bt) / 10.0F;
                    float const u = 1.0F - t;
                    float const sx =
                        (u * u * u * static_cast<float>(-tw)) + (3.0F * u * u * t * static_cast<float>(-tw)) +
                        (3.0F * u * t * t * static_cast<float>(-tw + jf)) + (t * t * t * static_cast<float>(-tw + jf));
                    float const sy = (u * u * u * static_cast<float>(jf)) + (3.0F * u * u * t * 0.0F) +
                                     (3.0F * u * t * t * 0.0F) + (t * t * t * 0.0F);
                    float fx       = sx;
                    float fy       = sy;
                    rotatePoint(fx, fy, dir);
                    pts.emplace_back(cx + static_cast<int>(fx), cy + static_cast<int>(fy));
                }
            }
        }

        // ── Full outline generation ──────────────────────────────────────
        void genFullOutline(
            PointV& out,
            int bodyX,
            int bodyY,
            int bodyW,
            int bodyH,
            int cr,
            int tw,
            int th,
            int tcX,
            int tcY,
            SpeechBubble::TailDirection dir,
            SpeechBubble::TailProfile const & profile,
            SpeechBubble::BubbleStyle style)
        {
            out.clear();
            out.reserve(160);
            if (bodyW <= 0 || bodyH <= 0) {
                return;
            }

            // ── Non-rectangular body styles ─────────────────────────────
            int const ccx = bodyX + (bodyW / 2);
            int const ccy = bodyY + (bodyH / 2);
            int const crx = bodyW / 2;
            int const cry = bodyH / 2;

            if (style == SpeechBubble::BubbleStyle::Round) {
                if (dir == SpeechBubble::TailDirection::None) {
                    addEllipseOutline(out, ccx, ccy, crx, cry, 0.0F, 2.0F * PI, 64);
                    return;
                }
                float theta0 = 0.5F * PI;
                float refR   = static_cast<float>(crx);
                switch (dir) {
                case SpeechBubble::TailDirection::Down:
                    theta0 = 0.5F * PI;
                    refR   = static_cast<float>(crx);
                    break;
                case SpeechBubble::TailDirection::Up:
                    theta0 = 1.5F * PI;
                    refR   = static_cast<float>(crx);
                    break;
                case SpeechBubble::TailDirection::Left:
                    theta0 = PI;
                    refR   = static_cast<float>(cry);
                    break;
                case SpeechBubble::TailDirection::Right:
                    theta0 = 0.0F;
                    refR   = static_cast<float>(cry);
                    break;
                case SpeechBubble::TailDirection::BottomLeft:
                    theta0 = 0.75F * PI;
                    refR   = static_cast<float>(std::min(crx, cry));
                    break;
                case SpeechBubble::TailDirection::BottomRight:
                    theta0 = 0.25F * PI;
                    refR   = static_cast<float>(std::min(crx, cry));
                    break;
                case SpeechBubble::TailDirection::TopLeft:
                    theta0 = 1.25F * PI;
                    refR   = static_cast<float>(std::min(crx, cry));
                    break;
                case SpeechBubble::TailDirection::TopRight:
                    theta0 = 1.75F * PI;
                    refR   = static_cast<float>(std::min(crx, cry));
                    break;
                default:
                    break;
                }
                float const ux = std::cos(theta0);
                float const uy = std::sin(theta0);
                float spread   = 0.25F;
                if (refR > 0.5F) {
                    spread = std::clamp(std::asin(std::min(static_cast<float>(tw) / refR, 1.0F)), 0.12F, 0.5F);
                }
                int const N      = 64;
                float const step = ((2.0F * PI) - (2.0F * spread)) / static_cast<float>(N - 1);
                for (int i = 0; i < N; ++i) {
                    float const a = theta0 + spread + (step * static_cast<float>(i));
                    out.emplace_back(
                        ccx + static_cast<int>(static_cast<float>(crx) * std::cos(a)),
                        ccy + static_cast<int>(static_cast<float>(cry) * std::sin(a)));
                }
                float const ax = static_cast<float>(ccx) + (static_cast<float>(crx) * std::cos(theta0));
                float const ay = static_cast<float>(ccy) + (static_cast<float>(cry) * std::sin(theta0));
                out.emplace_back(
                    static_cast<int>(ax + (ux * static_cast<float>(th))),
                    static_cast<int>(ay + (uy * static_cast<float>(th))));
                return;
            }

            if (style == SpeechBubble::BubbleStyle::Thought) {
                addCloudOutline(out, ccx, ccy, crx, cry, 80);
                return;
            }

            if (style == SpeechBubble::BubbleStyle::Shout) {
                addBurstOutline(out, ccx, ccy, bodyW, bodyH, 8);
                return;
            }

            // ── Rounded rectangle body (Classic / Whisper) ─────────────

            int const cr2 = std::min({cr, bodyW / 2, bodyH / 2});

            int const tlx  = bodyX + cr2;
            int const tly  = bodyY + cr2;
            int const trx  = bodyX + bodyW - 1 - cr2;
            int const try_ = bodyY + cr2;
            int const brx  = bodyX + bodyW - 1 - cr2;
            int const bry  = bodyY + bodyH - 1 - cr2;
            int const blx  = bodyX + cr2;
            int const bly  = bodyY + bodyH - 1 - cr2;

            int const btmEdge = bodyY + bodyH - 1;
            int const topEdge = bodyY;
            int const lftEdge = bodyX;
            int const rgtEdge = bodyX + bodyW - 1;

            auto const dirIdx = [&]() -> int {
                switch (dir) {
                case SpeechBubble::TailDirection::Down:
                    return 0;
                case SpeechBubble::TailDirection::Up:
                    return 1;
                case SpeechBubble::TailDirection::Left:
                    return 2;
                case SpeechBubble::TailDirection::Right:
                    return 3;
                default:
                    return 0;
                }
            }();

            if (dir == SpeechBubble::TailDirection::None) {
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                // Remove consecutive duplicates and trailing first==last
                if (out.size() >= 2) {
                    size_t writeIdx = 0;
                    for (size_t i = 1; i < out.size(); ++i) {
                        if (out[i].x != out[writeIdx].x || out[i].y != out[writeIdx].y) {
                            ++writeIdx;
                            if (writeIdx != i) {
                                out[writeIdx] = out[i];
                            }
                        }
                    }
                    out.resize(writeIdx + 1);
                    if (out.size() >= 2 && out.front().x == out.back().x && out.front().y == out.back().y) {
                        out.pop_back();
                    }
                }
                return;
            }

            switch (dir) {
            case SpeechBubble::TailDirection::Down: {
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                // BR arc ends at (brx, btmEdge) — connect diagonally to tail

                addSpineTail(out, tcX, btmEdge, profile, tw, th, dirIdx, brx, blx);

                // Left transition in addSpineTail already ends at (blx, btmEdge)
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                break;
            }
            case SpeechBubble::TailDirection::Up: {
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                // BL arc ends at (lftEdge, bly)
                out.emplace_back(lftEdge, try_);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);

                addSpineTail(out, tcX, topEdge, profile, tw, th, dirIdx);

                out.emplace_back(tlx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                break;
            }
            case SpeechBubble::TailDirection::Left: {
                out.emplace_back(brx, btmEdge);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                // BL arc ends at (lftEdge, bly) — tail follows

                addSpineTail(out, lftEdge, tcY, profile, tw, th, dirIdx);

                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                // TR arc ends at (rgtEdge, bodyY+cr2)
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                break;
            }
            case SpeechBubble::TailDirection::Right: {
                out.emplace_back(tlx, topEdge);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                // TR arc ends at (rgtEdge, bodyY+cr2) — tail follows

                addSpineTail(out, rgtEdge, tcY, profile, tw, th, dirIdx);

                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                // BL arc ends at (lftEdge, bly)
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                break;
            }
            case SpeechBubble::TailDirection::BottomLeft: {
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addCornerTail(out, blx, bly, cr2, tw, th, 90.0F, 180.0F);
                break;
            }
            case SpeechBubble::TailDirection::BottomRight: {
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addCornerTail(out, brx, bry, cr2, tw, th, 0.0F, 90.0F);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                break;
            }
            case SpeechBubble::TailDirection::TopLeft: {
                out.emplace_back(lftEdge, bodyY + cr2);
                addCornerTail(out, tlx, tly, cr2, tw, th, 180.0F, 270.0F);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                break;
            }
            case SpeechBubble::TailDirection::TopRight: {
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addCornerTail(out, trx, try_, cr2, tw, th, 270.0F, 360.0F);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                break;
            }
            case SpeechBubble::TailDirection::Auto:
            case SpeechBubble::TailDirection::None:
            default:
                out.emplace_back(lftEdge, bodyY + cr2);
                addArc(out, tlx, tly, cr2, PI, 1.5F * PI);
                out.emplace_back(trx, topEdge);
                addArc(out, trx, try_, cr2, 1.5F * PI, 2.0F * PI);
                out.emplace_back(rgtEdge, bodyY + cr2);
                out.emplace_back(rgtEdge, bry);
                addArc(out, brx, bry, cr2, 0.0F, 0.5F * PI);
                out.emplace_back(blx, btmEdge);
                addArc(out, blx, bly, cr2, 0.5F * PI, PI);
                break;
            }

            // Remove consecutive duplicate points (zero-length edges break ear-clipping)
            if (out.size() >= 2) {
                size_t writeIdx = 0;
                for (size_t i = 1; i < out.size(); ++i) {
                    if (out[i].x != out[writeIdx].x || out[i].y != out[writeIdx].y) {
                        ++writeIdx;
                        if (writeIdx != i) {
                            out[writeIdx] = out[i];
                        }
                    }
                }
                out.resize(writeIdx + 1);
                // Remove last point if it equals first (polygon is implicitly closed)
                if (out.size() >= 2 && out.front().x == out.back().x && out.front().y == out.back().y) {
                    out.pop_back();
                }
            }
        }

    } // anonymous namespace

    // ── Public API ───────────────────────────────────────────────────

    SpeechBubble::SpeechBubble()
    {
        setBorderSize(0);
        setOpaque(true);
    }

    SpeechBubble::~SpeechBubble() = default;

    void SpeechBubble::setCornerRadius(unsigned int radius)
    {
        mCornerRadius = radius;
    }
    unsigned int SpeechBubble::getCornerRadius() const
    {
        return mCornerRadius;
    }
    void SpeechBubble::setTailWidth(unsigned int width)
    {
        mTailWidth = width;
    }
    unsigned int SpeechBubble::getTailWidth() const
    {
        return mTailWidth;
    }
    void SpeechBubble::setBubbleStyle(BubbleStyle style)
    {
        mBubbleStyle = style;
    }
    SpeechBubble::BubbleStyle SpeechBubble::getBubbleStyle() const
    {
        return mBubbleStyle;
    }
    void SpeechBubble::setTailHeight(unsigned int height)
    {
        mTailHeight = height;
    }
    unsigned int SpeechBubble::getTailHeight() const
    {
        return mTailHeight;
    }
    void SpeechBubble::setTailDirection(TailDirection direction)
    {
        mTailDirection = direction;
    }
    SpeechBubble::TailDirection SpeechBubble::getTailDirection() const
    {
        return mTailDirection;
    }
    void SpeechBubble::setTailProfile(TailProfile const & profile)
    {
        mTailProfile = profile;
    }
    SpeechBubble::TailProfile SpeechBubble::getTailProfile() const
    {
        return mTailProfile;
    }
    void SpeechBubble::setTailOffset(float offset)
    {
        mTailOffset = std::max(0.0F, std::min(1.0F, offset));
    }
    float SpeechBubble::getTailOffset() const
    {
        return mTailOffset;
    }
    void SpeechBubble::setTailColor(Color color)
    {
        mTailColor = color;
    }

    Color SpeechBubble::getTailColor() const
    {
        if (mTailColor == Color{}) {
            return getBaseColor();
        }
        return mTailColor;
    }

    Rectangle SpeechBubble::getChildrenArea()
    {
        TailDirection dir = mTailDirection;
        if (dir == TailDirection::Auto) {
            dir = TailDirection::Down;
        }
        int const th = static_cast<int>(mTailHeight);
        int ti       = 0;
        int bi       = 0;
        int li       = 0;
        int ri       = 0;
        if (dir == TailDirection::Down) {
            bi += th;
        } else if (dir == TailDirection::Up) {
            ti += th;
        } else if (dir == TailDirection::Left) {
            li += th;
        } else if (dir == TailDirection::Right) {
            ri += th;
        } else if (dir == TailDirection::BottomLeft) {
            li += th;
            bi += th;
        } else if (dir == TailDirection::BottomRight) {
            ri += th;
            bi += th;
        } else if (dir == TailDirection::TopLeft) {
            li += th;
            ti += th;
        } else if (dir == TailDirection::TopRight) {
            ri += th;
            ti += th;
        }
        return {li, ti, std::max(0, getWidth() - li - ri), std::max(0, getHeight() - ti - bi)};
    }

    void SpeechBubble::adjustSize()
    {
        Container::adjustSize();
        int const minW = static_cast<int>((2 * mCornerRadius) + 40);
        int const minH = static_cast<int>((2 * mCornerRadius) + mTailHeight + 20);
        if (getWidth() < minW) {
            setWidth(minW);
        }
        if (getHeight() < minH) {
            setHeight(minH);
        }
    }

    void SpeechBubble::draw(Graphics* graphics)
    {
        int const w = getWidth();
        int const h = getHeight();

        TailDirection dir = mTailDirection;
        if (dir == TailDirection::Auto) {
            dir = TailDirection::Down;
        }

        int const cr = static_cast<int>(mCornerRadius);
        int const th = static_cast<int>(mTailHeight);
        int const tw = static_cast<int>(mTailWidth) / 2;

        bool const rectStyle = (mBubbleStyle == BubbleStyle::Classic || mBubbleStyle == BubbleStyle::Whisper);
        bool const tailBody  = rectStyle || mBubbleStyle == BubbleStyle::Round;

        int bodyX = 0;
        int bodyY = 0;
        int bodyW = w;
        int bodyH = h;
        switch (dir) {
        case TailDirection::Down:
            if (tailBody) {
                bodyH = h - th;
            }
            break;
        case TailDirection::Up:
            if (tailBody) {
                bodyY = th;
                bodyH = h - th;
            }
            break;
        case TailDirection::Left:
            if (tailBody) {
                bodyX = th;
                bodyW = w - th;
            }
            break;
        case TailDirection::Right:
            if (tailBody) {
                bodyW = w - th;
            }
            break;
        case TailDirection::BottomLeft:
            if (tailBody) {
                bodyX = th;
                bodyW = w - th;
                bodyH = h - th;
            }
            break;
        case TailDirection::BottomRight:
            if (tailBody) {
                bodyW = w - th;
                bodyH = h - th;
            }
            break;
        case TailDirection::TopLeft:
            if (tailBody) {
                bodyX = th;
                bodyY = th;
                bodyW = w - th;
                bodyH = h - th;
            }
            break;
        case TailDirection::TopRight:
            if (tailBody) {
                bodyY = th;
                bodyW = w - th;
                bodyH = h - th;
            }
            break;
        case TailDirection::None:
        default:
            break;
        }

        int const tcX = bodyX + static_cast<int>(static_cast<float>(bodyW) * mTailOffset);
        int const tcY = bodyY + static_cast<int>(static_cast<float>(bodyH) * mTailOffset);

        PointV outline;
        genFullOutline(outline, bodyX, bodyY, bodyW, bodyH, cr, tw, th, tcX, tcY, dir, mTailProfile, mBubbleStyle);

        if (outline.size() < 3) {
            return;
        }

        // Remove consecutive duplicates (tip cap can generate many identical vertices with r≤1)
        if (outline.size() >= 2) {
            size_t writeIdx = 0;
            for (size_t i = 1; i < outline.size(); ++i) {
                if (outline[i].x != outline[writeIdx].x || outline[i].y != outline[writeIdx].y) {
                    ++writeIdx;
                    if (writeIdx != i) {
                        outline[writeIdx] = outline[i];
                    }
                }
            }
            outline.resize(writeIdx + 1);
            if (outline.size() >= 2 && outline.front().x == outline.back().x && outline.front().y == outline.back().y) {
                outline.pop_back();
            }
        }

        // ── Fill ──
        Color bgColor;
        if (isFocused() &&
            ((getSelectionMode() & Widget::SelectionMode::Background) == Widget::SelectionMode::Background)) {
            bgColor = getSelectionColor();
        } else {
            bgColor = getBaseColor();
        }

        if (isOpaque() && outline.size() >= 3) {
            Color const tailColor   = getTailColor();
            bool const separateTail = rectStyle && (dir != TailDirection::None && tailColor != bgColor);

            // Ear-clipping triangulation (handles concave polygon with protruding tail)
            PointV tris;
            tris.reserve((outline.size() - 2) * 3);

            auto crossZ = [](Point const & a, Point const & b, Point const & c) -> int64_t {
                return (static_cast<int64_t>(b.x - a.x) * static_cast<int64_t>(c.y - a.y)) -
                       (static_cast<int64_t>(b.y - a.y) * static_cast<int64_t>(c.x - a.x));
            };

            auto pointInTriangle = [&crossZ](
                                       Point const & p, Point const & a, Point const & b, Point const & c) -> bool {
                int64_t const d1 = crossZ(p, a, b);
                int64_t const d2 = crossZ(p, b, c);
                int64_t const d3 = crossZ(p, c, a);
                bool const neg   = (d1 < 0) || (d2 < 0) || (d3 < 0);
                bool const pos   = (d1 > 0) || (d2 > 0) || (d3 > 0);
                return !(neg && pos);
            };

            PointV poly = outline;
            while (poly.size() > 3) {
                bool found = false;
                for (std::size_t i = 0; i < poly.size() && !found; ++i) {
                    std::size_t const prev = (i == 0) ? poly.size() - 1 : i - 1;
                    std::size_t const next = (i + 1 == poly.size()) ? 0 : i + 1;

                    int64_t const z = crossZ(poly[prev], poly[i], poly[next]);
                    if (z <= 0) {
                        continue; // not convex (reflex or colinear)
                    }

                    // Check no other vertex lies inside triangle (prev,i,next)
                    bool isEar = true;
                    for (std::size_t j = 0; j < poly.size(); ++j) {
                        if (j == prev || j == i || j == next) {
                            continue;
                        }
                        if (pointInTriangle(poly[j], poly[prev], poly[i], poly[next])) {
                            isEar = false;
                            break;
                        }
                    }
                    if (isEar) {
                        tris.push_back(poly[prev]);
                        tris.push_back(poly[i]);
                        tris.push_back(poly[next]);
                        poly.erase(poly.begin() + static_cast<std::ptrdiff_t>(i));
                        found = true;
                    }
                }
                if (!found) {
                    // Fallback: skip remaining (should not happen for valid simple polygon)
                    break;
                }
            }
            if (poly.size() == 3) {
                tris.push_back(poly[0]);
                tris.push_back(poly[1]);
                tris.push_back(poly[2]);
            }

            // ── Fill using ear-clipped triangles ──
            graphics->setColor(bgColor);
            for (std::size_t ti = 0; ti + 2 < tris.size(); ti += 3) {
                graphics->fillTriangle(
                    tris[ti].x, tris[ti].y, tris[ti + 1].x, tris[ti + 1].y, tris[ti + 2].x, tris[ti + 2].y);
            }

            // When the tail has its own colour, repaint the triangulation
            // restricted to the tail region. Triangles that straddle the
            // body/tail junction get drawn twice, but the clip keeps the
            // tail colour strictly inside the tail protrusion so it never
            // bleeds back up into the body.
            if (separateTail) {
                Rectangle tailRect(0, 0, 0, 0);
                switch (dir) {
                case TailDirection::Up:
                    tailRect = Rectangle(bodyX, bodyY - th, bodyW, th);
                    break;
                case TailDirection::Down:
                    tailRect = Rectangle(bodyX, bodyY + bodyH, bodyW, th);
                    break;
                case TailDirection::Left:
                    tailRect = Rectangle(bodyX - th, bodyY, th, bodyH);
                    break;
                case TailDirection::Right:
                    tailRect = Rectangle(bodyX + bodyW, bodyY, th, bodyH);
                    break;
                default:
                    break;
                }
                if (!tailRect.isEmpty()) {
                    graphics->pushClipArea(tailRect);
                    graphics->setColor(tailColor);
                    for (std::size_t ti = 0; ti + 2 < tris.size(); ti += 3) {
                        graphics->fillTriangle(
                            tris[ti].x - tailRect.x,
                            tris[ti].y - tailRect.y,
                            tris[ti + 1].x - tailRect.x,
                            tris[ti + 1].y - tailRect.y,
                            tris[ti + 2].x - tailRect.x,
                            tris[ti + 2].y - tailRect.y);
                    }
                    graphics->popClipArea();
                }
            }
        }

        // ── Thought-pattern tail dots ──
        if (isOpaque() && mBubbleStyle == BubbleStyle::Thought && dir != TailDirection::None && outline.size() >= 3) {
            float ux = 0.0F;
            float uy = 1.0F;
            switch (dir) {
            case TailDirection::Down:
                ux = 0.0F;
                uy = 1.0F;
                break;
            case TailDirection::Up:
                ux = 0.0F;
                uy = -1.0F;
                break;
            case TailDirection::Left:
                ux = -1.0F;
                uy = 0.0F;
                break;
            case TailDirection::Right:
                ux = 1.0F;
                uy = 0.0F;
                break;
            case TailDirection::BottomLeft:
                ux = -0.70710678F;
                uy = 0.70710678F;
                break;
            case TailDirection::BottomRight:
                ux = 0.70710678F;
                uy = 0.70710678F;
                break;
            case TailDirection::TopLeft:
                ux = -0.70710678F;
                uy = -0.70710678F;
                break;
            case TailDirection::TopRight:
                ux = 0.70710678F;
                uy = -0.70710678F;
                break;
            default:
                break;
            }
            std::array<float const, 3> const dotR = {4.0F, 4.0F, 3.0F};
            float const start                     = (static_cast<float>(std::min(bodyW, bodyH)) * 0.5F) + 6.0F;
            for (std::size_t i = 0; i < 3; ++i) {
                float const dist = start + (static_cast<float>(i + 1) * 9.0F);
                Point const c(
                    static_cast<int>(lroundf(static_cast<float>(tcX) + (ux * dist))),
                    static_cast<int>(lroundf(static_cast<float>(tcY) + (uy * dist))));
                graphics->setColor(bgColor);
                graphics->drawFillCircle(c, static_cast<unsigned int>(dotR[i]));
                if (getBorderSize() > 0) {
                    graphics->setColor(getBorderColor());
                    graphics->drawCircle(c, static_cast<unsigned int>(dotR[i]));
                }
            }
        }

        // ── Border ──
        if (getBorderSize() > 0) {
            Color borderColor;
            if (isFocused() &&
                ((getSelectionMode() & Widget::SelectionMode::Border) == Widget::SelectionMode::Border)) {
                borderColor = getSelectionColor();
            } else {
                borderColor = getBorderColor();
            }
            graphics->setColor(borderColor);
            if (mBubbleStyle == BubbleStyle::Whisper) {
                float const dashLen = 5.0F;
                float const gapLen  = 3.0F;
                for (std::size_t i = 0; i < outline.size(); ++i) {
                    Point const & a = outline[i];
                    Point const & b = outline[(i + 1) % outline.size()];
                    float const dx  = static_cast<float>(b.x - a.x);
                    float const dy  = static_cast<float>(b.y - a.y);
                    float const len = std::sqrt((dx * dx) + (dy * dy));
                    if (len <= 0.0F) {
                        continue;
                    }
                    float const vx = dx / len;
                    float const vy = dy / len;
                    for (float s = 0.0F; s < len;) {
                        float const s2 = std::min(s + dashLen, len);
                        graphics->drawLine(
                            a.x + static_cast<int>(vx * s),
                            a.y + static_cast<int>(vy * s),
                            a.x + static_cast<int>(vx * s2),
                            a.y + static_cast<int>(vy * s2));
                        s += dashLen + gapLen;
                    }
                }
            } else {
                for (std::size_t i = 0; i + 1 < outline.size(); ++i) {
                    graphics->drawLine(outline[i].x, outline[i].y, outline[i + 1].x, outline[i + 1].y);
                }
                graphics->drawLine(outline.back().x, outline.back().y, outline.front().x, outline.front().y);
            }
        }
    }

} // namespace fcn
