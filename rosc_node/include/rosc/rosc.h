/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * rosc.h
 *
 *  Created on: 20.01.2013
 *      Author: Christian Holl
 */

#ifndef ROSC_SPIN_H_
#define ROSC_SPIN_H_

#include <rosc/debug/debug_out.h>
#include <rosc/system/spec.h>
#include <rosc/system/types.h>

#ifndef __HOSTNAME_MAX_LEN__
	#warning __HOSTNAME_MAX_LEN__ undefined! Automatically set to 50.
	#define __HOSTNAME_MAX_LEN__ 50
#endif

#ifndef __NODENAME_MAX_LEN__
	#warning __NODENAME_MAX_LEN__ undefined! Automatically set to 50.
	#define __NODENAME_MAX_LEN__ 50
#endif

#ifndef __ROS_PARAMETER_MAX_LEN__
	#warning __ROS_PARAMETER_MAX_LEN__ undefined! Automatically set to 50.
	#define __ROS_PARAMETER_MAX_LEN__ 50
#endif


#ifdef __SYSTEM_HAS_MALLOC__
#error ROSC MALLOC IS NOT IMPLEMENTED YET THIS WILL ___NOT___ WORK ____ATM____!
#endif


#include <rosc/system/setup.h>
#include <rosc/system/rosc_spin.h>
#include <rosc/system/rosc_init.h>
#include <rosc/com_ifaces/iface.h>



#endif /* ROSC_SPIN_H_ */
