/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_VCL_INC_OPENGL_SALBMP_H
#define INCLUDED_VCL_INC_OPENGL_SALBMP_H

#include <vcl/opengl/OpenGLContext.hxx>

#include <opengl/texture.hxx>

#include <salbmp.hxx>

#include <memory>

struct  BitmapBuffer;
class   BitmapPalette;
namespace vcl
{
    class Kernel;
}

class VCL_PLUGIN_PUBLIC OpenGLSalBitmap final : public SalBitmap
{
private:
    OpenGLTexture                       maTexture;
    bool                                mbDirtyTexture;
    BitmapPalette                       maPalette;
    std::shared_ptr<sal_uInt8>          mpUserBuffer;
    sal_uInt16                          mnBits;
    sal_uInt32                          mnBytesPerRow;
    int                                 mnWidth;
    int                                 mnHeight;

    virtual void updateChecksum() const override;

    bool calcChecksumGL(OpenGLTexture& rInputTexture, BitmapChecksum& rChecksum) const;

public:
    OpenGLSalBitmap();
    virtual ~OpenGLSalBitmap() override;

public:

    // SalBitmap methods
    bool            Create( const Size& rSize, sal_uInt16 nBitCount, const BitmapPalette& rPal ) override;
    bool            Create( const SalBitmap& rSalBmp ) override;
    bool            Create( const SalBitmap& rSalBmp, SalGraphics* pGraphics ) override;
    bool            Create( const SalBitmap& rSalBmp, sal_uInt16 nNewBitCount ) override;
    virtual bool    Create( const css::uno::Reference< css::rendering::XBitmapCanvas >& rBitmapCanvas,
                            Size& rSize,
                            bool bMask = false ) override;

    void            Destroy() final override;

    Size            GetSize() const override;
    sal_uInt16      GetBitCount() const override;

    BitmapBuffer*   AcquireBuffer( BitmapAccessMode nMode ) override;
    void            ReleaseBuffer( BitmapBuffer* pBuffer, BitmapAccessMode nMode ) override;

    bool            GetSystemData( BitmapSystemData& rData ) override;

    bool            ScalingSupported() const override;
    bool            Scale( const double& rScaleX, const double& rScaleY, BmpScaleFlag nScaleFlag ) override;
    bool            Replace( const Color& rSearchColor, const Color& rReplaceColor, sal_uInt8 nTol ) override;
    bool            ConvertToGreyscale() override;
    bool            InterpretAs8Bit() override;

public:

    void            Create( const OpenGLTexture& rTex, long nX, long nY, long nWidth, long nHeight );
    OpenGLTexture&  GetTexture() const;
    const BitmapPalette& GetBitmapPalette() const { return maPalette; }

private:

    GLuint          CreateTexture();
    bool            AllocateUserData();
    void            DeallocateUserData();
    bool            ReadTexture();

private:

    bool ImplScaleFilter( const rtl::Reference< OpenGLContext > &xContext, const double& rScaleX, const double& rScaleY, GLenum nFilter );
    static void ImplCreateKernel( const double& fScale, const vcl::Kernel& rKernel, GLfloat*& pWeights, sal_uInt32& aKernelSize );
    bool ImplScaleConvolution(const rtl::Reference< OpenGLContext > &xContext, const double& rScaleX, const double& rScaleY, const vcl::Kernel& rKernel);
    bool ImplScaleArea( const rtl::Reference< OpenGLContext > &xContext,
                        double rScaleX, double rScaleY );

public:

    void ImplScale( const double& rScaleX, const double& rScaleY, BmpScaleFlag nScaleFlag );
};

#endif // INCLUDED_VCL_INC_OPENGL_SALBMP_H

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
