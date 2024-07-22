// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/hge/hgeimageloader.hpp"

#include "fifechan/exception.hpp"
#include "fifechan/hge/hgeimage.hpp"

namespace fcn
{
    HGE* HGEImageLoader::mHGE = NULL;

    HGEImageLoader::HGEImageLoader()
    {
        mHGE = hgeCreate(HGE_VERSION);
    }

    HGEImageLoader::~HGEImageLoader()
    {
        mHGE->Release();
    }

    Image* HGEImageLoader::load(std::string const & filename, bool convertToDisplayFormat)
    {
        HTEXTURE texture = mHGE->Texture_Load(filename.c_str());

        if (texture == NULL) {
            fcn::throwException(
                (std::string("Unable to load: ") + filename),
                static_cast<char const *>(__FUNCTION__),
                __FILE__,
                __LINE__);
        }

        HGEImage* image = new HGEImage(texture, true);

        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }

        return image;
    }

    Image* HGEImageLoader::load(hgeResourceManager* manager, char* name, int resgroup, bool convertToDisplayFormat)
    {
        HTEXTURE texture = manager->GetTexture(name, resgroup);

        if (texture == NULL) {
            fcn::throwException(
                (std::string("Unable to load: ") + name), static_cast<char const *>(__FUNCTION__), __FILE__, __LINE__);
        }

        HGEImage* image = new HGEImage(texture, true);

        if (convertToDisplayFormat) {
            image->convertToDisplayFormat();
        }

        return image;
    }
} // namespace fcn
