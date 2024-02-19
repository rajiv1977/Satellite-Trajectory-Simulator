#ifndef __IOBSERVER_H__
#define __IOBSERVER_H__

#pragma once

UISOFT_BEGIN_NAMESPACE(backend)

template <typename T>
interface iobserver
{
    virtual void notify_observer(T& xi_observable, void* xi_data) = 0;
};

UISOFT_END_NAMESPACE

#endif // __IOBSERVER_H__
