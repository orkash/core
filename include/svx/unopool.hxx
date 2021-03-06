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

#ifndef INCLUDED_SVX_UNOPOOL_HXX
#define INCLUDED_SVX_UNOPOOL_HXX

#include <com/sun/star/lang/XServiceInfo.hpp>
#include <comphelper/propertysethelper.hxx>
#include <cppuhelper/implbase4.hxx>
#include <svx/svxdllapi.h>

class SdrModel;
class SfxItemPool;

/** This class implements the service com.sun.star.drawing.Defaults.
    It works on the SfxItemPool from the given model and the global
    draw object item pool.
    The class can work in a read only mode without a model. Derivated
    classes can set a model on demand by overiding getModelPool().
*/
class SVX_DLLPUBLIC SvxUnoDrawPool :    public ::cppu::OWeakAggObject,
                        public ::com::sun::star::lang::XServiceInfo,
                        public ::com::sun::star::lang::XTypeProvider,
                        public comphelper::PropertySetHelper
{
public:
    SvxUnoDrawPool(SdrModel* pModel, sal_Int32 nServiceId);

    /** deprecated */
    SvxUnoDrawPool(SdrModel* pModel);
    virtual ~SvxUnoDrawPool() throw();

    /** This returns the item pool from the given model, or the default pool if there is no model and bReadOnly is true.
        If bReadOnly is false and there is no model the default implementation returns NULL.
    */
    virtual SfxItemPool* getModelPool( bool bReadOnly ) throw();

    // overriden helpers from comphelper::PropertySetHelper
    virtual void _setPropertyValues( const comphelper::PropertyMapEntry** ppEntries, const ::com::sun::star::uno::Any* pValues ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::beans::PropertyVetoException, ::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::lang::WrappedTargetException ) SAL_OVERRIDE;
    virtual void _getPropertyValues( const comphelper::PropertyMapEntry** ppEntries, ::com::sun::star::uno::Any* pValue ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException ) SAL_OVERRIDE;

    virtual void _getPropertyStates( const comphelper::PropertyMapEntry** ppEntries, ::com::sun::star::beans::PropertyState* pStates ) throw(::com::sun::star::beans::UnknownPropertyException ) SAL_OVERRIDE;
    virtual void _setPropertyToDefault( const comphelper::PropertyMapEntry* pEntry )  throw(::com::sun::star::beans::UnknownPropertyException ) SAL_OVERRIDE;
    virtual ::com::sun::star::uno::Any _getPropertyDefault( const comphelper::PropertyMapEntry* pEntry ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::WrappedTargetException ) SAL_OVERRIDE;

    // XInterface
    virtual ::com::sun::star::uno::Any SAL_CALL queryAggregation( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;
    virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type & rType ) throw(::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;
    virtual void SAL_CALL acquire() throw() SAL_OVERRIDE;
    virtual void SAL_CALL release() throw() SAL_OVERRIDE;

    // XTypeProvider
    virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes(  ) throw(::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;
    virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId(  ) throw(::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

    // XServiceInfo
    virtual OUString SAL_CALL getImplementationName() throw( ::com::sun::star::uno::RuntimeException, std::exception ) SAL_OVERRIDE;
    virtual sal_Bool SAL_CALL supportsService( const OUString& ServiceName ) throw( ::com::sun::star::uno::RuntimeException, std::exception ) SAL_OVERRIDE;
    virtual ::com::sun::star::uno::Sequence< OUString > SAL_CALL getSupportedServiceNames() throw( ::com::sun::star::uno::RuntimeException, std::exception ) SAL_OVERRIDE;

protected:
    void init();

    static void getAny( SfxItemPool* pPool, const comphelper::PropertyMapEntry* pEntry, ::com::sun::star::uno::Any& rValue ) throw(::com::sun::star::beans::UnknownPropertyException);
    virtual void putAny( SfxItemPool* pPool, const comphelper::PropertyMapEntry* pEntry, const ::com::sun::star::uno::Any& rValue ) throw(::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::lang::IllegalArgumentException, css::uno::RuntimeException);

protected:
    SdrModel* mpModel;
    SfxItemPool* mpDefaultsPool;
};

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
