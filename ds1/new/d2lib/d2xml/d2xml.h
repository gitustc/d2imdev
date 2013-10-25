/*
 * =====================================================================================
 *
 *       Filename: xmlwrp.h
 *        Created: 10/20/2013 06:41:23 AM
 *  Last Modified: 10/21/2013 08:21:03 PM
 *
 *    Description: xml operation wrapping
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */


typedef iks     D2XML;


#define d2xml_load( path, ptr )         (iks_load((path), (ptr)))
#define d2xml_delete( ptr )             (iks_delete(ptr))
#define d2xml_child( ptr )              (iks_child(ptr))
#define d2xml_next( ptr )               (iks_next(ptr))

#define d2xml_type( ptr )               (iks_type(ptr))
#define d2xml_name( ptr )               (iks_name(ptr))
#define d2xml_find_cdata( ptr, str )    (iks_find_cdata((ptr), (str))
