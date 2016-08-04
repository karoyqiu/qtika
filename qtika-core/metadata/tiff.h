/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "qtika-core-global.h"
#include "property.h"


namespace qtika {

namespace metadata {


/**
 * XMP Exif TIFF schema. This is a collection of
 * {@link Property property definition} constants for the Exif TIFF
 * properties defined in the XMP standard.
 *
 * @since Apache Tika 0.8
 * @see <a href="http://wwwimages.adobe.com/content/dam/Adobe/en/devnet/xmp/pdfs/cc-201306/XMPSpecificationPart2.pdf"
 *        >XMP Specification, Part 2: Standard Schemas</a>
 */
class QTIKACORESHARED_EXPORT TIFF
{
protected:
    TIFF() Q_DECL_EQ_DEFAULT;

public:
    virtual ~TIFF() Q_DECL_EQ_DEFAULT;

public:
    /**
     * "Number of bits per component in each channel."
     */
    static Property BITS_PER_SAMPLE();

    /**
     * "Image height in pixels."
     */
    static Property IMAGE_LENGTH();

    /**
     * "Image width in pixels."
     */
    static Property IMAGE_WIDTH();

    /**
     * "Number of components per pixel."
     */
    static Property SAMPLES_PER_PIXEL();

    /**
     * Did the Flash fire when taking this image?
     */
    static Property FLASH_FIRED();

    /**
     * "Exposure time in seconds."
     */
    static Property EXPOSURE_TIME();

    /**
     * "F-Number."
     * The f-number is the focal length divided by the "effective" aperture
     *  diameter. It is a dimensionless number that is a measure of lens speed.
     */
    static Property F_NUMBER();

    /**
     * "Focal length of the lens, in millimeters."
     */
    static Property FOCAL_LENGTH();

    /**
     * "ISO Speed and ISO Latitude of the input device as specified in ISO 12232"
     */
    static Property ISO_SPEED_RATINGS();

    /**
     * "Manufacturer of the recording equipment."
     */
    static Property EQUIPMENT_MAKE();

    /**
     * "Model name or number of the recording equipment."
     */
    static Property EQUIPMENT_MODEL();

    /**
     * "Software or firmware used to generate the image."
     */
    static Property SOFTWARE();

    /**
     * "The Orientation of the image."
     *  1 = 0th row at top, 0th column at left
     *  2 = 0th row at top, 0th column at right
     *  3 = 0th row at bottom, 0th column at right
     *  4 = 0th row at bottom, 0th column at left
     *  5 = 0th row at left, 0th column at top
     *  6 = 0th row at right, 0th column at top
     *  7 = 0th row at right, 0th column at bottom
     *  8 = 0th row at left, 0th column at bottom
     */
    static Property ORIENTATION();

    /**
     * "Horizontal resolution in pixels per unit."
     */
    static Property RESOLUTION_HORIZONTAL();

    /**
     * "Vertical resolution in pixels per unit."
     */
    static Property RESOLUTION_VERTICAL();

    /**
     * "Units used for Horizontal and Vertical Resolutions."
     * One of "Inch" or "cm"
     */
    static Property RESOLUTION_UNIT();

    /**
     * "Date and time when original image was generated"
     */
    static Property ORIGINAL_DATE();
};


}       // namespace metadata

}       // namespace qtika
