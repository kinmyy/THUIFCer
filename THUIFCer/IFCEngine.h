////////////////////////////////////////////////////////////////////////
//  Author:  Peter Bonsma
//  Date:    11 July 2008
//  Project: IFC Engine DLL
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@tno.nl
//
//  more info for using the IFC Engine DLL in other languages: 
//  see other examples or contact:  pim.vandenhelm@tno.nl
////////////////////////////////////////////////////////////////////////

/* disable the merging functionality by removing the */ 
/* _MERGING macro from the compiler options */
#ifndef _MERGING
#define _MERGING
#endif

#if !defined(AFX_IFCENGINE_H__E61DCDC8_CF8E_48DD_A8A3_C62AB6E95095__INCLUDED_)
#define AFX_IFCENGINE_H__E61DCDC8_CF8E_48DD_A8A3_C62AB6E95095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef __AFXWIN_H__
//	#error include 'stdafx.h' before including this file for PCH
//#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef __cplusplus
    typedef char bool; /* define the boolean type not available in c */
#endif

#define		sdaiADB					1
#define		sdaiAGGR				sdaiADB + 1
#define		sdaiBINARY				sdaiAGGR + 1
#define		sdaiBOOLEAN				sdaiBINARY + 1
#define		sdaiENUM				sdaiBOOLEAN + 1
#define		sdaiINSTANCE			sdaiENUM + 1
#define		sdaiINTEGER				sdaiINSTANCE + 1
#define		sdaiLOGICAL				sdaiINTEGER + 1
#define		sdaiREAL				sdaiLOGICAL + 1
#define		sdaiSTRING				sdaiREAL + 1

#define     sdaiARRAY               1
#define     sdaiLIST                2
#define     sdaiSET                 3
#define     sdaiBAG                 4

typedef	int				* SdaiAggr;
typedef	void			* SdaiAttr;
typedef	int				SdaiInstance;
typedef	int				SdaiInteger;
typedef	int				SdaiModel;
typedef	int				SdaiPrimitiveType;
typedef	int				* SdaiSet;
typedef	char			* SdaiString;

int __declspec(dllexport) __stdcall setStringUnicode(
										bool	unicode
									);

int __declspec(dllexport) * __stdcall xxxxGetEntityAndSubTypesExtent(	//		SdaiSet
										int		model,					//			SdaiModel
										int		entity					//			SdaiEntity
									);

int __declspec(dllexport) * __stdcall xxxxGetEntityAndSubTypesExtentBN(	//		SdaiAggr
										int		model,					//			SdaiModel
										char	* entityName			//			SdaiString
									);

bool __declspec(dllexport) __stdcall xxxxIsKindOf(int instance, int entity);



int __declspec(dllexport) * __stdcall xxxxGetInstancesUsing(			//		SdaiAggr
										int		instance				//			SdaiInstance
									);

int __declspec(dllexport) __stdcall xxxxDeleteFromAggregation(	        //		SdaiAggr
										int		instance,				//			SdaiInstance
										int		* aggregate,			//			SdaiAggr
										int		elementIndex			//			SdaiInteger
									);

void __declspec(dllexport) * __stdcall xxxxGetAttrDefinitionByValue(	//		SdaiAttr
										int		instance,				//			SdaiInstance
										void	* value		            //			void (aggregation or instance)
									);

void __declspec(dllexport) __stdcall xxxxGetAttrNameByIndex(          
                                        int		instance,				//			SdaiInstance
										int 	index,			        //			SdaiAttr
                                        char**  name                    //          name
                                    );


int __declspec(dllexport) __stdcall sdaiOpenModelBN(
										int		repository,
										char	* fileName,
										char	* schemaName
									);

int __declspec(dllexport) __stdcall sdaiCreateModelBN(
										int		repository,
										char	* fileName,
										char	* schemaName
									);

void __declspec(dllexport) __stdcall SetSPFFHeader(
                                        char    * description,
                                        char    * implementationLevel,
                                        char    * name,
                                        char    * timeStamp,
                                        char    * author,
                                        char    * organization,
                                        char    * preprocessorVersion,
                                        char    * originatingSystem,
                                        char    * authorization,
                                        char    * fileSchema
                                    );

void __declspec(dllexport) __stdcall sdaiSaveModelBN(
										int		model,
										char	* fileName
									);

void __declspec(dllexport) __stdcall sdaiSaveModelAsXmlBN(
										int		model,
										char	* fileName
									);

void __declspec(dllexport) __stdcall sdaiCloseModel(
										int		model
									);


void __declspec(dllexport) * __stdcall engiGetAggrElement(				//		void*
										int		* aggregate,			//			SdaiAggr
										int		elementIndex,			//			SdaiInteger
										int		valueType,				//			SdaiPrimitiveType
										void	* pValue				//			void*
									);

int __declspec(dllexport) __stdcall engiGetInstanceMetaInfo(			//		void*
										int		* instance,				//			SdaiAppInstance
										int		* localId,				//			...
										char	** className,			//			..
										char	** classNameUC			//			...*
									);

int __declspec(dllexport) __stdcall engiGetInstanceLocalId(				//
										int		instance				//			SdaiAppInstance
									);

char __declspec(dllexport) * __stdcall engiGetInstanceClassInfo(		//
										int		instance				//			SdaiAppInstance
									);

char __declspec(dllexport) * __stdcall engiGetInstanceClassInfoUC(		//
										int		instance				//			SdaiAppInstance
									);
	

void __declspec(dllexport) __stdcall sdaiAppend(						//		void
										int		list,					//			SdaiList
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

void __declspec(dllexport) __stdcall sdaiBeginning(						//		void
										int		iterator				//			SdaiIterator
									);

void __declspec(dllexport) * __stdcall sdaiCreateADB(					//		void*
										int		valueType,				//			SdaiPrimitive
										void	* value					//			void*
									);

int __declspec(dllexport) * __stdcall sdaiCreateAggr(					//		SdaiAggr
										int		instance,				//			SdaiAppInstance
										void	* attribute				//			SdaiAttr
									);

int __declspec(dllexport) * __stdcall sdaiCreateAggrBN(					//		SdaiAggr
										int		instance,				//			SdaiAppInstance
										char	* attributeName			//			SdaiString
									);

int __declspec(dllexport) __stdcall sdaiCreateInstance(					//		SdaiAppInstance
										int		model,					//			SdaiModel
										int		entity					//			SdaiEntity
									);

int __declspec(dllexport) __stdcall sdaiCreateInstanceEI(					//		SdaiAppInstance
										int		model,					//			SdaiModel
										int		entity,					//			SdaiEntity
										int		express_id				//			
									);

int __declspec(dllexport) __stdcall sdaiCreateInstanceBN(				//		SdaiAppInstance
										int		model,					//			SdaiModel
										char	* entityName			//			SdaiString
									);

int __declspec(dllexport) __stdcall sdaiCreateInstanceBNEI(				//		SdaiAppInstance
										int		model,					//			SdaiModel
										char	* entityName,			//			SdaiString
										int		express_id				//			
									);

void __declspec(dllexport) * __stdcall sdaiCreateIterator(				//		SdaiIterator
										int		* aggregate				//			SdaiAggr
									);

void __declspec(dllexport) __stdcall sdaiDeleteInstance(				//		void
										int		instance				//			SdaiAppInstance
									);

void __declspec(dllexport) __stdcall sdaiDeleteIterator(				//		void
										int		iterator				//			SdaiIterator
									);

void __declspec(dllexport) __stdcall sdaiEnd(							//		void
										int		iterator				//			SdaiIterator
									);

int __declspec(dllexport) __stdcall sdaiErrorQuery(						//		SdaiErrorCode
									);

void __declspec(dllexport) * __stdcall sdaiGetAggrByIterator(			//		void*
										int		iterator,				//			SdaiIterator
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

void __declspec(dllexport) * __stdcall sdaiGetAttr(						//		void*
										int		instance,				//			SdaiInstance
										void	* attribute,			//			SdaiAttr
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

void __declspec(dllexport) * __stdcall	sdaiGetAttrBN(					//		void*
										int		instance,				//			SdaiInstance
										char	* attributeName,		//			SdaiString
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

void __declspec(dllexport) * __stdcall sdaiGetAttrDefinition(			//		SdaiAttr
										int		entity,					//			SdaiEntity
										char	* attributeName			//			SdaiString
									);

int __declspec(dllexport) __stdcall	sdaiGetEntity(						//		SdaiEntity
										int		model,					//			SdaiModel
										char	* entityName			//			SdaiString
									);

int __declspec(dllexport) * __stdcall sdaiGetEntityExtent(				//		SdaiAggr
										int		model,					//			SdaiModel
										int		entity					//			SdaiEntity
									);

int __declspec(dllexport) * __stdcall sdaiGetEntityExtentBN(			//		SdaiAggr
										int		model,					//			SdaiModel
										char	* entityName			//			SdaiString
									);

int __declspec(dllexport) __stdcall sdaiGetInstanceType(				//		SdaiEntity
										int		instance				//			SdaiInstance
									);

int	__declspec(dllexport) __stdcall	sdaiGetMemberCount(					//		SdaiInteger
										int		* aggregate				//			SdaiAggr
									);

bool __declspec(dllexport) __stdcall sdaiIsInstanceOf(					//		SdaiBoolean
										int		instance,				//			SdaiInstance
										int		entity					//			SdaiEntity
									);

bool __declspec(dllexport) __stdcall sdaiIsInstanceOfBN(				//		SdaiBoolean
										int		instance,				//			SdaiInstance
										char	* entityName			//			SdaiString
									);

bool __declspec(dllexport) __stdcall sdaiNext(							//		SdaiBoolean
										int		iterator				//			SdaiIterator
									);

void __declspec(dllexport) __stdcall sdaiPrepend(						//		void
										int		list,					//			SdaiList
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

bool __declspec(dllexport) __stdcall sdaiPrevious(						//		SdaiBoolean
										int		iterator				//			SdaiIterator
									);

void __declspec(dllexport) __stdcall sdaiPutADBTypePath(				//		void
										void	* ADB,					//			sdaiADB
										int		pathCount,				//			int
										char	* path					//			char*
									);

void __declspec(dllexport) __stdcall sdaiPutAggrByIterator(				//		void
										int		iterator,				//			SdaiIterator
										int		valueType,				//			SdaiPrimitiveType
										void	* value					//			void*
									);

void __declspec(dllexport) __stdcall sdaiPutAttr(						//		void
										int		instance,				//			SdaiInstance
										void	* attribute,			//			SdaiAttr
										int		valueType,				//			SdaiPrimitive
										void	* value					//			void*
									);

void __declspec(dllexport) __stdcall sdaiPutAttrBN(						//		void
										int		instance,				//			SdaiInstance
										char	* attributeName,		//			SdaiString
										int		valueType,				//			SdaiPrimitive
										void	* value					//			void*
									);

bool __declspec(dllexport) __stdcall sdaiTestAttr(						//		SdaiBoolean
										int		instance,				//			SdaiInstance
										void	* attribute				//			SdaiAttr
									);

bool __declspec(dllexport) __stdcall sdaiTestAttrBN(					//		SdaiBoolean
										int		instance,				//			SdaiInstance
										char	* attributeName			//			SdaiString
									);


int __declspec(dllexport) __stdcall initializeModelling(
										int		model,
										int		* noVertices,
										int		* noIndices,
										double	scale
									);

int __declspec(dllexport) __stdcall initializeModellingInstance(
										int		model,
										int		* noVertices,
										int		* noIndices,
										double	scale,
										int		instance				//			SdaiInstance
									);

int __declspec(dllexport) __stdcall initializeModellingInstanceEx(
										int		model,
										int		* noVertices,
										int		* noIndices,
										double	scale,
										int		instance,				//			SdaiInstance
										int		instanceList			//			SdaiInstance
									);

int __declspec(dllexport) __stdcall finalizeModelling(
										int		model,
										float	* pVertices,
										int		* pIndices,
										int		FVF
									);

int __declspec(dllexport) __stdcall getInstanceInModelling(
										int		model,
										int		instance,
										int		mode,
										int		* startVertex,
										int		* startIndex,
										int		* primitiveCount
									);

int __declspec(dllexport) __stdcall getInstanceDerivedPropertiesInModelling(
										int		model,
										int		instance,
										double	* height,
										double	* width,
										double	* thickness
									);

int __declspec(dllexport) __stdcall getInstanceDerivedBoundingBox(
										int		model,
										int		instance,
										double	* Ox,
										double	* Oy,
										double	* Oz,
										double	* Vx,
										double	* Vy,
										double	* Vz
									);

int __declspec(dllexport) __stdcall getInstanceDerivedTransformationMatrix(
										int		model,
										int		instance,
										double	* _11,
										double	* _12,
										double	* _13,
										double	* _14,
										double	* _21,
										double	* _22,
										double	* _23,
										double	* _24,
										double	* _31,
										double	* _32,
										double	* _33,
										double	* _34,
										double	* _41,
										double	* _42,
										double	* _43,
										double	* _44
									);

void __declspec(dllexport) __stdcall circleSegments(
										int		circles,
										int		smallCircles
									);

int __declspec(dllexport) __stdcall getTimeStamp(
										int		model
									);

char __declspec(dllexport) * __stdcall getChangedData(
										int		model,
										int		* timeStamp
									);

void __declspec(dllexport) __stdcall setChangedData(
										int		model,
										int		* timeStamp,
										char	* changedData
									);

void __declspec(dllexport) * __stdcall internalGetBoundingBox(
										int		model,
										int		instance
									);

void __declspec(dllexport) * __stdcall internalGetCenter(
										int		model,
										int		instance
									);

void __declspec(dllexport) __stdcall internalSetLink(
										int		instance,				//			SdaiInstance
										char	* attributeName,		//			SdaiString
										int		linked_id
									);

void __declspec(dllexport) __stdcall internalAddAggrLink(				//		void
										int		list,					//			SdaiList
										int		linked_id
									);

char __declspec(dllexport) * __stdcall sdaiGetStringAttrBN(int instance, char * attributeName);

int __declspec(dllexport) __stdcall sdaiGetInstanceAttrBN(int instance, char * attributeName);

int __declspec(dllexport) __stdcall sdaiGetAggregationAttrBN(int instance, char * attributeName);



void __declspec(dllexport) __stdcall engiGetNotReferedAggr(int model, int * pValue);

void __declspec(dllexport) __stdcall engiGetAttributeAggr(int instance, int * pValue);

void __declspec(dllexport) __stdcall engiGetAggrUnknownElement(		//		void
										int		* aggregate,			//			SdaiAggr
										int		elementIndex,			//			SdaiInteger
										int		* valueType,			//			SdaiPrimitiveType
										void	* pValue				//			void*
									);

bool __declspec(dllexport) __stdcall sdaiIsKindOf(int instance, int entity);

int __declspec(dllexport) * __stdcall sdaiFindInstanceUsers(int instance, int* domain, int* resultList);

int __declspec(dllexport) * __stdcall sdaiFindInstanceUsedIn(int instance, int* domain, int* resultList);

int __declspec(dllexport) __stdcall sdaiplusGetAggregationType(int instance, int *aggregation);

#ifdef __cplusplus
}
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IFCENGINE_H__E61DCDC8_CF8E_48DD_A8A3_C62AB6E95095__INCLUDED_)

