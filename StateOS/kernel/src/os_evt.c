/******************************************************************************

    @file    StateOS: os_evt.c
    @author  Rajmund Szymanski
    @date    24.01.2018
    @brief   This file provides set of functions for StateOS.

 ******************************************************************************

   Copyright (c) 2018 Rajmund Szymanski. All rights reserved.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

 ******************************************************************************/

#include "inc/os_evt.h"

/* -------------------------------------------------------------------------- */
void evt_init( evt_t *evt )
/* -------------------------------------------------------------------------- */
{
	assert(!port_isr_inside());
	assert(evt);

	port_sys_lock();

	memset(evt, 0, sizeof(evt_t));

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
evt_t *evt_create( void )
/* -------------------------------------------------------------------------- */
{
	evt_t *evt;

	assert(!port_isr_inside());

	port_sys_lock();

	evt = core_sys_alloc(sizeof(evt_t));
	evt_init(evt);
	evt->res = evt;

	port_sys_unlock();

	return evt;
}

/* -------------------------------------------------------------------------- */
void evt_kill( evt_t *evt )
/* -------------------------------------------------------------------------- */
{
	assert(!port_isr_inside());
	assert(evt);

	port_sys_lock();

	core_all_wakeup(evt, E_STOPPED);

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
void evt_delete( evt_t *evt )
/* -------------------------------------------------------------------------- */
{
	port_sys_lock();

	evt_kill(evt);
	core_sys_free(evt->res);

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
static
unsigned priv_evt_wait( evt_t *evt, cnt_t time, unsigned(*wait)(void*,cnt_t) )
/* -------------------------------------------------------------------------- */
{
	unsigned event;

	assert(evt);

	port_sys_lock();

	event = wait(evt, time);

	port_sys_unlock();

	return event;
}

/* -------------------------------------------------------------------------- */
unsigned evt_waitUntil( evt_t *evt, cnt_t time )
/* -------------------------------------------------------------------------- */
{
	assert(!port_isr_inside());

	return priv_evt_wait(evt, time, core_tsk_waitUntil);
}

/* -------------------------------------------------------------------------- */
unsigned evt_waitFor( evt_t *evt, cnt_t delay )
/* -------------------------------------------------------------------------- */
{
	assert(!port_isr_inside());

	return priv_evt_wait(evt, delay, core_tsk_waitFor);
}

/* -------------------------------------------------------------------------- */
void evt_give( evt_t *evt, unsigned event )
/* -------------------------------------------------------------------------- */
{
	assert(evt);

	port_sys_lock();

	core_all_wakeup(evt, event);

	port_sys_unlock();
}

/* -------------------------------------------------------------------------- */
