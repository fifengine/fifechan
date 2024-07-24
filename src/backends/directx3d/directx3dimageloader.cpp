// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/backends/directx3d/directx3dimageloader.hpp"

#include "fifechan/backends/directx3d/directx3dimage.hpp"
#include "fifechan/exception.hpp"
#include <d3dx9.h>

namespace fcn
{
    DirectX3DImageLoader::DirectX3DImageLoader(LPDIRECT3DDEVICE9 device) : mDevice(device) { }

    Image* DirectX3DImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        LPDIRECT3DSURFACE9 surface = NULL;
        D3DXIMAGE_INFO imageInfo;
        HRESULT result;

        result = D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);

        if (result != D3D_OK) {
            fcn::throwException(
                ("Unable to load image " + filename), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        result = mDevice->CreateOffscreenPlainSurface(
            imageInfo.Width, imageInfo.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

        if (result != D3D_OK) {
            fcn::throwException(
                ("Unable to load image " + filename), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        result = D3DXLoadSurfaceFromFile(
            surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), &imageInfo);

        if (result != D3D_OK) {
            fcn::throwException(
                ("Unable to load image " + filename), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        return new DirectX3DImage(surface, mDevice, imageInfo.Width, imageInfo.Height, convertToDisplayFormat);
    }

    void DirectX3DImageLoader::setDevice(LPDIRECT3DDEVICE9 device)
    {
        mDevice = device;
    }
} // namespace fcn
