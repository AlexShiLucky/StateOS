/******************************************************************************

    @file    StateOS: os_mut.h
    @author  Rajmund Szymanski
    @date    14.03.2016
    @brief   This file contains definitions for StateOS.

 ******************************************************************************

    StateOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of StateOS distribution.

    StateOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    StateOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#pragma once

#include <oskernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : fast mutex (non-recursive and non-priority-inheritance)                                        *
 *                                                                                                                    *
 * Note              : use only to synchronize tasks with the same priority                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : OS_MUT                                                                                         *
 *                                                                                                                    *
 * Description       : define and initilize a mutex object                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : name of a pointer to mutex object                                                              *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define             OS_MUT( mut )                     \
                       mut_t mut##__mut = _MUT_INIT(); \
                       mut_id mut = & mut##__mut

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : static_MUT                                                                                     *
 *                                                                                                                    *
 * Description       : define and initilize a static mutex object                                                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : name of a pointer to mutex object                                                              *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define         static_MUT( mut )                     \
                static mut_t mut##__mut = _MUT_INIT(); \
                static mut_id mut = & mut##__mut

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : MUT_INIT                                                                                       *
 *                                                                                                                    *
 * Description       : create and initilize a mutex object                                                            *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return            : mutex object                                                                                   *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define                MUT_INIT() \
                      _MUT_INIT()

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : MUT_CREATE                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize a mutex object                                                            *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return            : pointer to mutex object                                                                        *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#ifndef __cplusplus
#define                MUT_CREATE() \
               &(mut_t)MUT_INIT()
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_create                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize a new mutex object                                                        *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return            : pointer to mutex object (mutex successfully created)                                           *
 *   0               : mutex not created (not enough free memory)                                                     *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

              mut_id   mut_create( void );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_kill                                                                                       *
 *                                                                                                                    *
 * Description       : reset the mutex object and wake up all waiting tasks with 'E_STOPPED' event value              *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     mut_kill( mut_id mut );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_waitUntil                                                                                  *
 *                                                                                                                    *
 * Description       : try to lock the mutex object,                                                                  *
 *                     wait until given timepoint if the mutex object can't be locked immediately                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *   time            : timepoint value                                                                                *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : mutex object was successfully locked                                                           *
 *   E_STOPPED       : mutex object was killed before the specified timeout expired                                   *
 *   E_TIMEOUT       : mutex object was not locked before the specified timeout expired                               *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

              unsigned mut_waitUntil( mut_id mut, unsigned time );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_waitFor                                                                                    *
 *                                                                                                                    *
 * Description       : try to lock the mutex object,                                                                  *
 *                     wait for given duration of time if the mutex object can't be locked immediately                *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *   delay           : duration of time (maximum number of ticks to wait for lock the mutex object)                   *
 *                     IMMEDIATE: don't wait if the mutex object can't be locked immediately                          *
 *                     INFINITE:  wait indefinitly until the mutex object has been locked                             *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : mutex object was successfully locked                                                           *
 *   E_STOPPED       : mutex object was killed before the specified timeout expired                                   *
 *   E_TIMEOUT       : mutex object was not locked before the specified timeout expired                               *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

              unsigned mut_waitFor( mut_id mut, unsigned delay );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_wait                                                                                       *
 *                                                                                                                    *
 * Description       : try to lock the mutex object,                                                                  *
 *                     wait indefinitly if the mutex object can't be locked immediately                               *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : mutex object was successfully locked                                                           *
 *   E_STOPPED       : mutex object was killed                                                                        *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline unsigned mut_wait( mut_id mut ) { return mut_waitFor(mut, INFINITE); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_take                                                                                       *
 *                                                                                                                    *
 * Description       : try to lock the mutex object,                                                                  *
 *                     don't wait if the mutex object can't be locked immediately                                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : mutex object was successfully locked                                                           *
 *   E_TIMEOUT       : mutex object can't be locked immediately                                                       *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline unsigned mut_take( mut_id mut ) { return mut_waitFor(mut, IMMEDIATE); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : mut_give                                                                                       *
 *                                                                                                                    *
 * Description       : try to unlock the mutex object (only owner task can unlock mutex object),                      *
 *                     don't wait if the mutex object can't be unlocked                                               *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   mut             : pointer to mutex object                                                                        *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : mutex object was successfully unlocked                                                         *
 *   E_TIMEOUT       : mutex object can't be unlocked                                                                 *
 *                                                                                                                    *
 * Note              : use only in thread mode                                                                        *
 *                                                                                                                    *
 **********************************************************************************************************************/

              unsigned mut_give( mut_id mut );

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus

class FastMutex : public __mut, private SafeEvent<__mut>
{
public:

	constexpr explicit
	FastMutex( void ): __mut(_MUT_INIT()) {}

	void     kill     ( void )            {        mut_kill     (this);         }
	unsigned waitUntil( unsigned _time  ) { return mut_waitUntil(this, _time);  }
	unsigned waitFor  ( unsigned _delay ) { return mut_waitFor  (this, _delay); }
	unsigned wait     ( void )            { return mut_wait     (this);         }
	unsigned take     ( void )            { return mut_take     (this);         }
	unsigned give     ( void )            { return mut_give     (this);         }
};

#endif

/* -------------------------------------------------------------------------- */
