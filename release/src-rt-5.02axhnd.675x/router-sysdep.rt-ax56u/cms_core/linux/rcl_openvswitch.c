/***********************************************************************
 *
 *  Copyright (c) 2006-2009  Broadcom 
 *  All Rights Reserved
 *
<:label-BRCM:2012:proprietary:standard

 This program is the proprietary software of Broadcom  and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").  Except as set forth in
 an Authorized License, Broadcom grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and Broadcom
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of Broadcom
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
:>
 *
 ************************************************************************/


#ifdef DMP_X_BROADCOM_COM_OPENVSWITCH_1

#include "odl.h"
#include "cms_core.h"
#include "cms_util.h"
#include "cms_msg.h"
#include "rcl.h"
#include "rut_system.h"
#include "rut_util.h"
#include "rut_openvswitch.h"


/*!\file rcl_openvswitch.c
 * \brief This file contains openvswitch related functions.
 *
 */

CmsRet rcl_openvswitchCfgObject( _OpenvswitchCfgObject *newObj,
                                 const _OpenvswitchCfgObject *currObj,
                                 const InstanceIdStack *iidStack __attribute__((unused)),
                                 char **errorParam __attribute__((unused)),
                                 CmsRet *errorCode __attribute__((unused)))
{
   CmsRet ret =CMSRET_SUCCESS;

   if (ENABLE_NEW_OR_ENABLE_EXISTING(newObj, currObj))
   {
      /* on system init, start openvswitch if it's enabled */
      ret = rutOpenVS_updateOpenvswitch(FALSE,"");
   }
   else if (POTENTIAL_CHANGE_OF_EXISTING(newObj, currObj))
   {
      if (newObj->enable !=  currObj->enable)
      {
         ret = rutOpenVS_updateOpenvswitch(TRUE, currObj->ifNameList);
      }
      else if ( newObj->enable)
      {
         if (cmsUtl_strcmp(newObj->OFControllerIPAddress, currObj->OFControllerIPAddress) ||
             (newObj->OFControllerPortNumber != currObj->OFControllerPortNumber))
         {
            ret = rutOpenVS_updateOpenvswitchOFController(newObj->OFControllerIPAddress,newObj->OFControllerPortNumber);
         }
         if (cmsUtl_strcmp(newObj->ifNameList, currObj->ifNameList))
         {
            ret = rutOpenVS_updateOpenvswitchPorts(currObj->ifNameList,newObj->ifNameList);
         }
      }
   }
   else if (DELETE_OR_DISABLE_EXISTING(newObj, currObj))
   {
      if(currObj->enable == TRUE) 
      {
         rutOpenVS_updateOpenvswitchPorts(currObj->ifNameList,"");
         ret = rutOpenVS_stopOpenvswitch();
      }
   }
   return ret;
}
#endif
