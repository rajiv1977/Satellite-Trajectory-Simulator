#ifndef __IOBSERVABLE_H__
#define __IOBSERVABLE_H__

#pragma once

#include <framework/iobserver.hpp>

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T>
interface iobservable
{
    virtual void register_observer(iobserver<T>* xi_observer) = 0;
    virtual void unregister_observer(iobserver<T>* xi_observer) = 0;

    virtual void notify_observers(void* xi_data) = 0;
};

UISOFT_END_NAMESPACE

#endif // __IOBSERVABLE_H__
