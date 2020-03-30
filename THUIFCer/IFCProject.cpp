#include "stdafx.h"
#include "IFCProject.h"
#include "misc.h"

/************************************************************************/
/*    IFC������ʵ��                                                     */
/************************************************************************/

//Ĭ�Ϲ��캯��
CIFCProject::CIFCProject()
{
	FilePath="";
	FileName="";
	ProjectID=0;
	IFCSchemaVersion="";
	IFCSchemaName="";
	ProjectName="";
	GlobalId="";
	Owner="";
	bOpen=FALSE;
	IFCGroups.clear();
	IFCGroupsMap.clear();
	CurrentGroupID=0;
	numVertices=0;
	numIndices=0;
	pVertices=NULL;
	pIndices=NULL;
	ModelType=0;
	XmlEntityTree=NULL;
	XmlFloorTree=NULL;

	ifcRelDefinesByType_TYPE=0;
	ifcRelDefinesByProperties_TYPE=0;
	ifcPropertySet_TYPE=0;
	ifcComplexProperty_TYPE=0;
	ifcPropertySingleValue_TYPE=0;
	ifcPropertyEnumeratedValue_TYPE=0;
	ifcPropertyBoundedValue_TYPE=0;
	ifcPropertyTableValue_TYPE=0;
	ifcPropertyReferenceValue_TYPE=0;
	ifcPropertyListValue_TYPE=0;
}

CIFCProject::CIFCProject(CString filePath,CString ifcSchemaName)
{
	FilePath=filePath;
	FileName=filePath.Right(filePath.GetLength()-filePath.ReverseFind('\\')-1);
	IFCSchemaName=ifcSchemaName;
	IFCSchemaVersion=_T("IFC2X3_TC1"); //��ʱ������ifc2x3
	ProjectID=0;
	ProjectName="";
	GlobalId="";
	Owner="";
	bOpen=FALSE;
	IFCGroups.clear();
	IFCGroupsMap.clear();
	CurrentGroupID=0;
	numVertices=0;
	numIndices=0;
	pVertices=NULL;
	pIndices=NULL;
	ModelType=1;
	XmlEntityTree=NULL;
	XmlFloorTree=NULL;
	XmlPropertyList=NULL;
}

//����
CIFCProject::~CIFCProject()
{
	if (pVertices)
		free(pVertices);
	if (pIndices)
		free(pIndices);

	if (XmlEntityTree)
	{
		delete XmlEntityTree;
		XmlEntityTree=NULL;
	}
	if (XmlFloorTree)
	{
		delete XmlFloorTree;
		XmlFloorTree=NULL;
	}

	if (XmlPropertyList)
	{
		delete XmlPropertyList;
		XmlPropertyList=NULL;
	}

	IFCGroups.clear();
	IFCGroupsMap.clear();
	if (ProjectID>0)
		sdaiCloseModel(ProjectID);
}

//����IFCEngine API������IFCģ��
BOOL CIFCProject::OpenIFCProject()
{
	if (FilePath==_T("")||IFCSchemaName==_T(""))
		return FALSE;

	//�Ѿ�����һ������
	if (ProjectID>0)
	{
		if (pVertices)
			free(pVertices);
		if (pIndices)
			free(pIndices);

		IFCGroups.clear();
		IFCGroupsMap.clear();
		CurrentGroupID=0;
		sdaiCloseModel(ProjectID);  /*IFCEngine API*/
		ProjectID = 0;
	}


	//Unicode -> Ansi
	CStringA strFilePath(FilePath);
	CStringA strIfcSchemaName(IFCSchemaName);
	//��IFCģ��
	ProjectID = sdaiOpenModelBN(1, (char *)(LPCSTR)strFilePath, (char *)(LPCSTR)strIfcSchemaName);  /*IFCEngine API*/

	if (ProjectID)
	{
		bOpen=TRUE;
		//TODO
		//��ȡIFC������
		//��ȡ����GloalId
		//��ȡOwner

		Initialize(); //��ʼ��
	}
	else
	{
		bOpen=FALSE;
	}

	return bOpen;
}

//����IFCEngine API������IFCģ��
BOOL CIFCProject::OpenIFCProject(CString filePath,CString ifcSchemaName)
{
	FilePath=filePath;
	FileName=filePath.Right(filePath.GetLength()-filePath.ReverseFind('\\')-1);
	IFCSchemaName=ifcSchemaName;

	if (FilePath==_T("")||IFCSchemaName==_T(""))
		return FALSE;

	//�Ѿ�����һ������
	if (ProjectID>0)
	{
		if (pVertices)
			free(pVertices);
		if (pIndices)
			free(pIndices);

		IFCGroups.clear();
		IFCGroupsMap.clear();
		CurrentGroupID=0;
		sdaiCloseModel(ProjectID);  /*IFCEngine API*/
		ProjectID = 0;
	}

	//Unicode -> Ansi
	CStringA strFilePath(FilePath);
	CStringA strIfcSchemaName(IFCSchemaName);
	//��IFCģ��
	ProjectID = sdaiOpenModelBN(1, (char *)(LPCSTR)strFilePath, (char *)(LPCSTR)strIfcSchemaName);  /*IFCEngine API*/

	if (ProjectID)
	{
		bOpen=TRUE;
		//TODO
		//��ȡIFC������
		//��ȡ����GloalId
		//��ȡOwner

		Initialize(); //��ʼ��
	}
	else
	{
		bOpen=FALSE;
	}

	return bOpen;
}

//����һ���Ѿ��򿪵Ĺ��̵ľ����������
BOOL CIFCProject::CopyIFCProject(int project)
{
	if (project<=0)
		return FALSE;

	CloseIFCProject();

	ProjectID=project;
	//TODO
	//��ȡIFC������
	//��ȡ����GloalId
	//��ȡOwner

	bOpen=TRUE;
	Initialize(); //��ʼ��

	return TRUE;
}

BOOL CIFCProject::SaveIFCProject()
{
	CStringA defaultPath(FilePath);
	sdaiSaveModelBN(ProjectID,(char *)(LPCSTR)defaultPath); /*IFCEngine API*/
	return TRUE;
}

//����IFCEngine API��������IFCģ��
BOOL CIFCProject::SaveIFCProject(CString filePath)
{
	CStringA newPath(filePath);
	sdaiSaveModelBN(ProjectID,(char *)(LPCSTR)newPath); /*IFCEngine API*/
	return TRUE;
}

//����IFCEngine API�����ر�IFCģ��
BOOL CIFCProject::CloseIFCProject()
{
	if (pVertices)
	{
		free(pVertices);
		pVertices=NULL;
	}
	if (pIndices)
	{
		free(pIndices);
		pIndices=NULL;
	}

	IFCGroups.clear();
	IFCGroupsMap.clear();
	CurrentGroupID=0;
	if (ProjectID>0)
		sdaiCloseModel(ProjectID);
	ProjectID=0;
	bOpen=FALSE;

	FilePath="";
	FileName="";
	IFCSchemaVersion="";
	IFCSchemaName="";
	ProjectName="";
	GlobalId="";
	Owner="";

	return TRUE;
}

//�½�һ���հ׹���
BOOL CIFCProject::CreateIFCProject(CString filePath,CString ifcSchemaName)
{
	if (bOpen)
		CloseIFCProject();

	FilePath=filePath;
	FileName=filePath.Right(filePath.GetLength()-filePath.ReverseFind('\\')-1);
	IFCSchemaName=ifcSchemaName;
	IFCSchemaVersion=_T("IFC2X3_TC1"); //��ʱ������ifc2x3
	if (FilePath==_T("")||IFCSchemaName==_T(""))
		return FALSE;

	//Unicode -> Ansi
	CStringA strFilePath(FilePath);
	CStringA strIfcSchemaName(IFCSchemaName);
	//����IFCģ��
	ProjectID = sdaiCreateModelBN(0, (char *)(LPCSTR)strFilePath, (char *)(LPCSTR)strIfcSchemaName);  /*IFCEngine API*/

	if (ProjectID<=0)
	{
		bOpen=FALSE;
	}
	else
	{
		bOpen=TRUE;
		//TODO
	}

	return bOpen;
}

//��ʼ��
BOOL CIFCProject::Initialize()
{
	if (bOpen)
	{
		//��ȡ3άģ�Ͷ������
		initializeModelling(ProjectID, &numVertices, &numIndices, 1); /*IFCEngine API*/
		//����ռ�
		pVertices = (float *) malloc(numVertices * sizeof(CUSTOMVERTEX));
		pIndices = (int *) malloc(numIndices * sizeof(int));
		//���3άģ�Ͷ�������
		finalizeModelling(ProjectID, &pVertices[0], &pIndices[0], D3DFVF_CUSTOMVERTEX); /*IFCEngine API*/
		

		//��ȡEntity��Ϣ
		RetrieveEntities(_T("IFCANNOTATION"), _T("Annotations"), _T("Annotations"));                                   //ע��
		RetrieveEntities(_T("IFCBEAM"), _T("Beams"), _T("Beams"));                                                 //��
		RetrieveEntities(_T("IFCBUILDING"), _T("Buildings"), _T("Buildings"));                                       //����
		RetrieveEntities(_T("IFCBUILDINGELEMENTPART"), _T("Building Element Parts"), _T("Building Element Parts"));//
		RetrieveEntities(_T("IFCBUILDINGELEMENTPROXY"), _T("Building Element Proxies"), _T("Building Element Proxies"));        //ר���豸
		RetrieveEntities(_T("IFCBUILDINGSTOREY"), _T("Building Stories"), _T("Building Stories"));                          //¥��
		RetrieveEntities(_T("IFCCHAMFEREDGEFEATURE"), _T("Chamfer Edge Features"), _T("Chamfer Edge Features"));//
		RetrieveEntities(_T("IFCCOLUMN"), _T("Columns"), _T("Columns"));                                           //����
		RetrieveEntities(_T("IFCCOVERING"), _T("Coverings"), _T("Coverings"));                                     //�ڸ���
		RetrieveEntities(_T("IFCCURTAINWALL"), _T("Curtain Walls"), _T("Curtain Walls"));                                //Ļǽ
		RetrieveEntities(_T("IFCDISCRETEACCESSORY"), _T("Discrete Accessories"), _T("Discrete Accessories"));               //
		RetrieveEntities(_T("IFCDISTRIBUTIONCHAMBERELEMENT"), _T("Distribution Chamber Elements"), _T("Distribution Chamber Elements"));//
		RetrieveEntities(_T("IFCDISTRIBUTIONCONTROLELEMENT"), _T("Distribution Control Elements"), _T("Distribution Control Elements"));//
		RetrieveEntities(_T("IFCDISTRIBUTIONELEMENT"), _T("Distribution Elements"), _T("Distribution Elements"));//
		RetrieveEntities(_T("IFCDISTRIBUTIONFLOWELEMENT"), _T("Distribution Flow Elements"), _T("Distribution Flow Elements"));//
		RetrieveEntities(_T("IFCDISTRIBUTIONPORT"), _T("Distribution Ports"), _T("Distribution Ports"));        //
		RetrieveEntities(_T("IFCDOOR"), _T("Doors"), _T("Doors"));                                                 //��
		RetrieveEntities(_T("IFCELECTRICALELEMENT"), _T("Electrical Elements"), _T("Electrical Elements"));                //�����豸
		RetrieveEntities(_T("IFCELECTRICDISTRIBUTIONPOINT"), _T("Electric Distribution Points"), _T("Electric Distribution Points"));//
		RetrieveEntities(_T("IFCELEMENTASSEMBLY"), _T("Element Assemblies"), _T("Element Assemblies"));         //
		RetrieveEntities(_T("IFCENERGYCONVERSIONDEVICE"), _T("Energy Conversion Devices"), _T("Energy Conversion Devices"));//
		RetrieveEntities(_T("IFCEQUIPMENTELEMENT"), _T("Equipment Elements"), _T("Equipment Elements"));        //
		RetrieveEntities(_T("IFCFASTENER"), _T("Fasteners"), _T("Fasteners"));                                       //����
		RetrieveEntities(_T("IFCFLOWCONTROLLER"), _T("Flow Controllers"), _T("Flow Controllers"));              //
		RetrieveEntities(_T("IFCFLOWFITTING"), _T("Flow Fittings"), _T("Flow Fittings"));                       //
		RetrieveEntities(_T("IFCFLOWMOVINGDEVICE"), _T("Flow Moving Devices"), _T("Flow Moving Devices"));      //
		RetrieveEntities(_T("IFCFLOWSEGMENT"), _T("Flow Segments"), _T("Flow Segments"));                       //
		RetrieveEntities(_T("IFCFLOWSTORAGEDEVICE"), _T("Flow Storage Devices"), _T("Flow Storage Devices"));   //
		RetrieveEntities(_T("IFCFLOWTERMINAL"), _T("Flow Terminals"), _T("Flow Terminals"));                    //
		RetrieveEntities(_T("IFCFLOWTREATMENTDEVICE"), _T("Flow Treatment Devices"), _T("Flow Treatment Devices"));//
		RetrieveEntities(_T("IFCFOOTING"), _T("Footings"), _T("Footings"));                                         //����
		RetrieveEntities(_T("IFCFURNISHINGELEMENT"), _T("Furnishing Elements"), _T("Furnishing Elements"));                    //�Ҿ�
		RetrieveEntities(_T("IFCGRID"), _T("Grids"), _T("Grids"));                                               //
		RetrieveEntities(_T("IFCMECHANICALFASTENER"), _T("Mechanical Fasteners"), _T("Mechanical Fasteners"));              //��е����
		RetrieveEntities(_T("IFCMEMBER"), _T("Members"), _T("Members"));                                         //�ṹ��
		RetrieveEntities(_T("IFCOPENINGELEMENT"), _T("Opening Elements"), _T("Opening Elements"));                          //����
		RetrieveEntities(_T("IFCPILE"), _T("Piles"), _T("Piles"));                                               //�ܵ�
		RetrieveEntities(_T("IFCPLATE"), _T("Plates"), _T("Plates"));                                               //��
		RetrieveEntities(_T("IFCPROJECTIONELEMENT"), _T("Projection Elements"), _T("Projection Elements"));     //
		RetrieveEntities(_T("IFCPROXY"), _T("Proxies"), _T("Proxies"));                                         //
		RetrieveEntities(_T("IFCRAILING"), _T("Railings"), _T("Railings"));                                         //����
		RetrieveEntities(_T("IFCRAMP"), _T("Ramps"), _T("Ramps"));                                               //�µ�
		RetrieveEntities(_T("IFCRAMPFLIGHT"), _T("Ramp Flights"), _T("Ramp Flights"));                                //�µ���
		RetrieveEntities(_T("IFCREINFORCINGBAR"), _T("Reinforcing Bars"), _T("Reinforcing Bars"));              //
		RetrieveEntities(_T("IFCREINFORCINGMESH"), _T("Reinforcing Meshes"), _T("Reinforcing Meshes"));         //
		RetrieveEntities(_T("IFCROOF"), _T("Roofs"), _T("Roofs"));                                               //�ݶ�
		RetrieveEntities(_T("IFCROUNDEDEDGEFEATURE"), _T("Rounded Edge Features"), _T("Rounded Edge Features"));//
		RetrieveEntities(_T("IFCSITE"), _T("Sites"), _T("Sites"));                                               //����
		RetrieveEntities(_T("IFCSLAB"), _T("Slabs"), _T("Slabs"));                                                 //��
		RetrieveEntities(_T("IFCSPACE"), _T("Spaces"), _T("Spaces"));                                             //�ռ�
		RetrieveEntities(_T("IFCSTAIR"), _T("Stairs"), _T("Stairs"));                                             //¥��
		RetrieveEntities(_T("IFCSTAIRFLIGHT"), _T("Stair Flights"), _T("Stair Flights"));                              //¥�ݶ�
		RetrieveEntities(_T("IFCSTRUCTURALCURVECONNECTION"), _T("Structural Curve Connections"), _T("Structural Curve Connections"));//
		RetrieveEntities(_T("IFCSTRUCTURALCURVEMEMBER"), _T("Structural Curve Members"), _T("Structural Curve Members"));//
		RetrieveEntities(_T("IFCSTRUCTURALCURVEMEMBERVARYING"), _T("Structural Curve Member Varyings"), _T("Structural Curve Member Varyings"));//
		RetrieveEntities(_T("IFCSTRUCTURALLINEARACTION"), _T("Structural Linear Actions"), _T("Structural Linear Actions"));//
		RetrieveEntities(_T("IFCSTRUCTURALLINEARACTIONVARYING"), _T("Structural Linear Action Varyings"), _T("Structural Linear Action Varyings"));//
		RetrieveEntities(_T("IFCSTRUCTURALPLANARACTION"), _T("Structural Planar Actions"), _T("Structural Planar Actions"));//
		RetrieveEntities(_T("IFCSTRUCTURALPLANARACTIONVARYING"), _T("Structural Planar Action Varyings"), _T("Structural Planar Action Varyings"));//
		RetrieveEntities(_T("IFCSTRUCTURALPOINTACTION"), _T("Structural Point Actions"), _T("Structural Point Actions"));//
		RetrieveEntities(_T("IFCSTRUCTURALPOINTCONNECTION"), _T("Structural Point Connections"), _T("Structural Point Connections"));//
		RetrieveEntities(_T("IFCSTRUCTURALPOINTREACTION"), _T("Structural Point Reactions"), _T("Structural Point Reactions"));//
		RetrieveEntities(_T("IFCSTRUCTURALSURFACECONNECTION"), _T("Structural Surface Connections"), _T("Structural Surface Connections"));//
		RetrieveEntities(_T("IFCSTRUCTURALSURFACEMEMBER"), _T("Structural Surface Members"), _T("Structural Surface Members"));//
		RetrieveEntities(_T("IFCSTRUCTURALSURFACEMEMBERVARYING"), _T("Structural Surface Member Varyings"), _T("Structural Surface Member Varyings"));//
		RetrieveEntities(_T("IFCTENDON"), _T("Tendons"), _T("Tendons"));                                        //
		RetrieveEntities(_T("IFCTENDONANCHOR"), _T("Tendon Anchors"), _T("Tendon Anchors"));                    //
		RetrieveEntities(_T("IFCTRANSPORTELEMENT"), _T("Transport Elements"), _T("Transport Elements"));        //
		RetrieveEntities(_T("IFCVIRTUALELEMENT"), _T("Virtual Elements"), _T("Virtual Elements"));                      //���⹹��
		RetrieveEntities(_T("IFCWALL"), _T("Walls"), _T("Walls"));                                                 //ǽ
		RetrieveEntities(_T("IFCWALLSTANDARDCASE"), _T("Walls Standard Cases"), _T("Walls Standard Cases"));                  //��׼ǽ
		RetrieveEntities(_T("IFCWINDOW"), _T("Windows"), _T("Windows"));                                             //��

		InitializeIfcType();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CIFCProject::InitializeIfcType()
{
	if (ProjectID<=0)
		return;

	ifcRelDefinesByType_TYPE	    = sdaiGetEntity(ProjectID, "IFCRELDEFINESBYTYPE");       /*IFCEngine API*/
	ifcRelDefinesByProperties_TYPE  = sdaiGetEntity(ProjectID, "IFCRELDEFINESBYPROPERTIES"); /*IFCEngine API*/
	ifcPropertySet_TYPE             = sdaiGetEntity(ProjectID, "IFCPROPERTYSET");            /*IFCEngine API*/
	ifcComplexProperty_TYPE			= sdaiGetEntity(ProjectID, "IFCCOMPLEXPROPERTY");        /*IFCEngine API*/
	ifcPropertySingleValue_TYPE		= sdaiGetEntity(ProjectID, "IFCPROPERTYSINGLEVALUE");    /*IFCEngine API*/
	ifcPropertyEnumeratedValue_TYPE	= sdaiGetEntity(ProjectID, "IFCPROPERTYENUMERATEDVALUE");/*IFCEngine API*/
	ifcPropertyBoundedValue_TYPE	= sdaiGetEntity(ProjectID, "IFCPROPERTYBOUNDEDVALUE");   /*IFCEngine API*/
	ifcPropertyTableValue_TYPE		= sdaiGetEntity(ProjectID, "IFCPROPERTYTABLEVALUE");     /*IFCEngine API*/
	ifcPropertyReferenceValue_TYPE	= sdaiGetEntity(ProjectID, "IFCPROPERTYREFERENCEVALUE"); /*IFCEngine API*/
	ifcPropertyListValue_TYPE		= sdaiGetEntity(ProjectID, "IFCPROPERTYLISTVALUE");      /*IFCEngine API*/
}

int CIFCProject::MakeID()
{
	CurrentGroupID++;
	return CurrentGroupID;
}
//����IFCEngine API������ȡʵ������
/*std::map  : һ��һӳ����������� */
/*std::pair : һ��Ԫ��             */
BOOL CIFCProject::RetrieveEntities(CString ObjectSPFFName/*e.g. IFCWINDOWS*/, CString ObjectGroupName/*e.g. Windows*/, CString ObjectGroupNameCN/*e.g. ��*/)
{
	int	*pID;

	//Unicode -> Ansi
	/*
	int len=WideCharToMultiByte(CP_ACP,0,ObjectSPFFName,-1,NULL,0,NULL,NULL);
	char *szObjectSPFFName=new char[len+1];
	WideCharToMultiByte(CP_ACP,0,ObjectSPFFName,-1,szObjectSPFFName,len,NULL,NULL);
	szObjectSPFFName[len]='\0';
	*/
	CStringA szObjectSPFFName(ObjectSPFFName);

	//pID��������һ��ʵ������֣���API�������أ�ע��Ϊָ��
	pID = sdaiGetEntityExtentBN(ProjectID, (char*)(LPCSTR)szObjectSPFFName); /*IFCEngine API*/

	//����һ����
	int theGroupID=MakeID();
	CIFCEntity Group(ObjectSPFFName,ObjectGroupName,ObjectGroupNameCN,pID,theGroupID);
	//��ʼ��
	BOOL bSucceed=Group.Initialize(ProjectID,GetModelType());
	if (!bSucceed)
		return FALSE;
	else
	{
		IFCGroups.insert(Entities_Pair(ObjectSPFFName,Group));  //����
		IFCGroupsMap.insert(EntityMap_Pair(theGroupID,ObjectSPFFName));
		return TRUE;
	}
}

BOOL CIFCProject::OpenFlag()
{
	return bOpen;
}

CString CIFCProject::GetIFCFilePath()
{
	return FilePath;
}

CString CIFCProject::GetIFCSchemaPath()
{
	return IFCSchemaName;
}

BOOL CIFCProject::SetPath(CString filePath, CString ifcSchemaName)
{
	if (bOpen)
		return FALSE;

	FilePath=filePath;
	FileName=filePath.Right(filePath.GetLength()-filePath.ReverseFind('\\')-1);
	IFCSchemaName=ifcSchemaName;
	IFCSchemaVersion=_T("IFC2X3_TC1"); //��ʱ������ifc2x3

	return TRUE;
}
int CIFCProject::GetProjectID()
{
	return ProjectID;
}

CIFCEntity CIFCProject::GetEntity(CString EntityName)
{
	if (bOpen)
	{
		if (!(IFCGroups.empty()))
		{
			Entities_Iter theIter;
			theIter=IFCGroups.find(EntityName);
			return theIter->second;
		}
	}

	CIFCEntity NULLEntity; //һ���ն���
	return NULLEntity;
}

CIFCEntity CIFCProject::GetEntity(int GroupID)
{
	CString entityname=FindEntityName(GroupID);
	if (entityname!=_T(""))
	{
		return GetEntity(entityname);
	}

	CIFCEntity NULLEntity; //һ���ն���
	return NULLEntity;
}

const CIFCEntity* CIFCProject::GetIFCEntity(CString GroupName)
{
	return NULL;
}

const CIFCEntity* CIFCProject::GetIFCEntity(int GroupID)
{
	return NULL;
}

BOOL CIFCProject::SetEntity(CString EntityName, CIFCEntity ifcEntity)
{
	if (!bOpen||(EntityName==_T("")))
	{
		return FALSE;
	}
	else
	{
		Entities_Iter theIter;
		theIter=IFCGroups.find(EntityName);
		if (theIter==IFCGroups.end())  //û�ҵ�
		{
			IFCGroups.insert(Entities_Pair(EntityName,ifcEntity));
		}
		else
		{
			IFCGroups[EntityName]=ifcEntity;
			/*
			IFCGroups.erase(theIter);
			IFCGroups.insert(Entities_Pair(EntityName,ifcEntity));
			*/
		}

		return TRUE;
	}
}

BOOL CIFCProject::SetEntitySelected(CString EntityName, BOOL viewable)
{
	if (!bOpen)
	{
		return FALSE;
	}
	else
	{
		Entities_Iter entitiesIter;
		entitiesIter=IFCGroups.find(EntityName);
		if (entitiesIter==IFCGroups.end())  //û�ҵ�
		{
			return FALSE;
		}
		else
		{
			CIFCEntity theEntity = entitiesIter->second;
			theEntity.SetAllSelected(viewable);
			/*
			IFCGroups.erase(EntityName);
			IFCGroups.insert(Entities_Pair(EntityName,theEntity));
			*/
			IFCGroups[EntityName]=theEntity;
			return TRUE;
		}
	}
}

BOOL CIFCProject::SetEntitySelected(int GroupID, BOOL viewable)
{
	CString EntityName=FindEntityName(GroupID);
	return SetEntitySelected(EntityName,viewable);
}

BOOL CIFCProject::SetAllSelected(BOOL viewalbe)
{
	if (!bOpen)
	{
		return FALSE;
	}
	else
	{
		Entities_Iter entitiesIter=IFCGroups.begin();
		for (;entitiesIter!=IFCGroups.end();++entitiesIter)
		{
			CString EntityName = entitiesIter->first;
			CIFCEntity theEntity = entitiesIter->second;
			theEntity.SetAllSelected(viewalbe);
			IFCGroups[EntityName]=theEntity;
		}
		return TRUE;
	}
}

BOOL CIFCProject::SetInstanceSelected(CString EntityName,int InstanceID, BOOL viewalbe)
{
	if (!bOpen)
		return FALSE;

	Entities_Iter entitiesIter;
	entitiesIter=IFCGroups.find(EntityName);
	if (entitiesIter==IFCGroups.end())
		return FALSE;

	CIFCEntity theEntity=entitiesIter->second;
	if (theEntity.SetInstanceSelected(InstanceID,viewalbe)==TRUE)
	{
		IFCGroups[EntityName]=theEntity;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CIFCProject::SetInstanceSelected(int GroupID,int InstanceID, BOOL viewalbe)
{
	CString EntityName=FindEntityName(GroupID);
	return SetInstanceSelected(EntityName,InstanceID,viewalbe);
}

int* CIFCProject::GetEntityID(CString EntityName)
{
	if (bOpen&&(ProjectID>0))
	{
		int	*GroupID;
		CStringA szObjectSPFFName(EntityName);
		GroupID = sdaiGetEntityExtentBN(ProjectID, (char*)(LPCSTR)szObjectSPFFName); /*IFCEngine API*/
		return GroupID;
	}

	return NULL;
}

int* CIFCProject::GetEntityID(int GroupID)
{
	return NULL;
}

int CIFCProject::GetInstanceNumByAPI(int *groupID)
{
	if (groupID)
	{
		if (bOpen&&(ProjectID>0))
		{
			return sdaiGetMemberCount(groupID); /*IFCEngine API*/
		}
	}
	
	return 0;
}

int CIFCProject::GetInstanceNumByAPI(CString EntityName)
{
	int *groupid;
	groupid = GetEntityID(EntityName);
	return GetInstanceNumByAPI(groupid);
}

void CIFCProject::SetModelType(int modeltype)
{
	ModelType=modeltype;
}

int CIFCProject::GetModelType()
{
	return ModelType;
}

int CIFCProject::GetNumVertices()
{
	return numVertices;
}

int CIFCProject::GetNumIndices()
{
	return numIndices;
}

const float* CIFCProject::GetpVertices()
{
	return pVertices;
}

const int* CIFCProject::GetpIndices()
{
	return pIndices;
}

const EntityGroup* CIFCProject::GetEntityGroup()
{
	if (IFCGroups.empty())
		return NULL;

	return &IFCGroups;
}

const TiXmlDocument* CIFCProject::GetEntityTree(BOOL reMake)
{
	if (IFCGroups.empty())
		return NULL;

	if (reMake||(NULL==XmlEntityTree))
	{
		if (XmlEntityTree)
		{
			delete XmlEntityTree;
			XmlEntityTree=NULL;
		}

		XmlEntityTree=new TiXmlDocument;
		TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
		XmlEntityTree->LinkEndChild(dec);
		TiXmlElement *pRoot=new TiXmlElement("Project");
		CStringA ProjectNameA(ProjectName); //�����������ڵ�
		pRoot->SetAttribute("ProjectName", "Building"/*(LPCSTR)ProjectNameA*/);

		//����
		Entities_constIter entitiesIter=IFCGroups.begin();
		for (;entitiesIter!=IFCGroups.end();++entitiesIter)
		{
			CIFCEntity entities=entitiesIter->second;
			TiXmlElement *pGroupElement=new TiXmlElement("Entity");
			int GroupID=entities.GetGroupID();
			CStringA NameCN(entities.GetNameCN());
			CStringA NameIFC(entities.GetNameIFC());
			CStringA NameEn(entities.GetEntityName());
			pGroupElement->SetAttribute("GroupID",GroupID);
			pGroupElement->SetAttribute("GroupName",(LPCSTR)NameEn);
			pGroupElement->SetAttribute("GroupNameIFC",(LPCSTR)NameIFC);
			pGroupElement->SetAttribute("GroupNameCN",(LPCSTR)NameCN);
			pGroupElement->SetAttribute("Number",entities.GetNumber());
			pGroupElement->SetAttribute("NumberSelected",entities.GetNumSelected());

			const InstanceGroup *pInstanceGroup=entities.GetInstanceGroup();
			Instance_constIter InstanceIter=pInstanceGroup->begin();
			for (;InstanceIter!=pInstanceGroup->end();++InstanceIter)
			{
				CIFCInstance instance=InstanceIter->second;
				TiXmlElement *pInstanceElement=new TiXmlElement("Instance");
				CStringA InstanceName(instance.GetName());
				CStringA GlobalID(instance.GetGlobalID());
				CStringA OID;
				OID.Format("%d",instance.GetOID());
				OID.Insert(0,'#');
				pInstanceElement->SetAttribute("InstanceID",instance.GetID());
				pInstanceElement->SetAttribute("InstanceName",(LPCSTR)InstanceName);
				pInstanceElement->SetAttribute("GlobalID",(LPCSTR)GlobalID);
				pInstanceElement->SetAttribute("OID",(LPCSTR)OID);
				pInstanceElement->SetAttribute("Selected",(int)instance.GetSelected());
				pGroupElement->LinkEndChild(pInstanceElement);
			}
			pRoot->LinkEndChild(pGroupElement);
		}
		XmlEntityTree->LinkEndChild(pRoot);
	}

	return XmlEntityTree;
}

const TiXmlDocument* CIFCProject::GetFloorTree(BOOL reMake)
{
	if (IFCGroups.empty())
		return NULL;

	if (XmlFloorTree)
	{
		delete XmlFloorTree;
		XmlFloorTree=NULL;
	}

	XmlFloorTree=new TiXmlDocument;
	return XmlFloorTree;
}

BOOL CIFCProject::SaveEnityTree(CString FileName,BOOL reMake)
{
	//���utf-8�����xml�ļ�
	if (!bOpen)
		return FALSE;

	if ((!XmlEntityTree)||reMake)
	{
		GetEntityTree(TRUE);
	}

	if (XmlEntityTree)
	{
		TiXmlPrinter printer;
		XmlEntityTree->Accept(&printer);
		const CStringW UnicodeStr(printer.CStr());
		const CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
		CFile theFile;
		theFile.Open(FileName,CFile::modeCreate|CFile::modeWrite);
		char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
		theFile.Write(&UTF8BOM,3);
		theFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
		theFile.Close();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CIFCProject::SaveFloorTree(CString FileName,BOOL reMake)
{
	return TRUE;
}

void CIFCProject::OutputUTF8XmlTest()
{
	XmlEntityTree=new TiXmlDocument;
	TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
	XmlEntityTree->LinkEndChild(dec);
	TiXmlElement *pRoot=new TiXmlElement("test");
	pRoot->SetAttribute("name","����");
	XmlEntityTree->LinkEndChild(pRoot);
	TiXmlPrinter printer;
	XmlEntityTree->Accept(&printer);

	if (1)
	{
		const CStringW UnicodeStr(printer.CStr());
		const CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);

		CFile theFile;
		theFile.Open(_T("test.xml"),CFile::modeCreate|CFile::modeWrite);
		char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
		theFile.Write(&UTF8BOM,3);
		theFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
		theFile.Close();
	}
	else
	{
		WCHAR *unicode;
		int lenth=strlen(printer.CStr());
		unicode=new WCHAR[lenth+1];
		AnsiToUnicode(printer.CStr(),unicode);

		char *utf8;
		utf8=(char*)malloc((lenth+1)*3);
		int utf8Length;
		utf8Length=UnicodeToUTF8(unicode,utf8);
		char *szUTF8;
		szUTF8=new char[utf8Length+1];
		memcpy(szUTF8,utf8,utf8Length);
		szUTF8[utf8Length]='\0';

		if (unicode)
		{
			delete unicode;
			unicode=NULL;
		}
		if (utf8)
		{
			free(utf8);
			utf8=NULL;
		}

		CFile theFile;
		theFile.Open(_T("test.xml"),CFile::modeCreate|CFile::modeWrite);
		char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
		theFile.Write(&UTF8BOM,3);
		theFile.Write(szUTF8,strlen(szUTF8));
		theFile.Close();

		if (szUTF8)
		{
			delete szUTF8;
			szUTF8=NULL;
		}
	}
}

CString CIFCProject::FindEntityName(int GroupID)
{
	EntityMap_constIter iter=IFCGroupsMap.find(GroupID);
	if (iter!=IFCGroupsMap.end())
	{
		return iter->second;
	}
	else
	{
		return _T("");
	}
}

int CIFCProject::GetInstanceNum(CString EntityName)
{
	if (EntityName!=_T(""))
		return GetEntity(EntityName).GetNumber();
	else
		return 0;
}

int CIFCProject::GetInstanceNum(int GroupID)
{
	if (GroupID>0)
		return GetInstanceNum(FindEntityName(GroupID));
	else
		return 0;
}

int CIFCProject::GetInstanceNumSelected(CString EntityName)
{
	if (EntityName!=_T(""))
		return GetEntity(EntityName).GetNumSelected();
	else
		return 0;
}

int CIFCProject::GetInstanceNumSelected(int GroupID)
{
	if (GroupID>0)
		return GetInstanceNumSelected(FindEntityName(GroupID));
	else
		return 0;
}

const TiXmlDocument* CIFCProject::GetPropertySet(int InstanceID)
{
	if (!bOpen)
		return NULL;

	if (IFCGroups.empty())
		return NULL;

	if (InstanceID<=0)
		return NULL;

	if (XmlPropertyList)
	{
		delete XmlPropertyList;
		XmlPropertyList=NULL;
	}

	XmlPropertyList=new TiXmlDocument;
	TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
	XmlPropertyList->LinkEndChild(dec);
	TiXmlElement *pRoot=new TiXmlElement("InstancePropertySet");
	pRoot->SetAttribute("InstanceID",InstanceID);
	//pRoot->SetAttribute("Name","");
	//pRoot->SetAttribute("GUID","");

	int *ifcRelDefiness,numIfcRelDefiness;
	//ȡ�ù�����ϵ�ļ��ϵľ�����������ϵ��ܼ���λ�൱��Entity��
	sdaiGetAttrBN(InstanceID,"IsDefinedBy",sdaiAGGR,&ifcRelDefiness); /*IFCEngine API*/
	//ȡ�ù�����ϵ�ĸ���
	numIfcRelDefiness=sdaiGetMemberCount(ifcRelDefiness); /*IFCEngine API*/

	int ifcInstance_TYPE;

	int ifcRelDefines; //һ��������ϵ�ľ��
	for (int i=0;i<numIfcRelDefiness;++i)
	{
		//ȡ�ù�����ϵ�ľ����ע�⣺��IFC��ϵ����Instance��
		engiGetAggrElement(ifcRelDefiness,i,sdaiINSTANCE,&ifcRelDefines); /*IFCEngine API*/
		//ȡ�ù�����ϵ������
		ifcInstance_TYPE=sdaiGetInstanceType(ifcRelDefines); /*IFCEngine API*/

		if (ifcInstance_TYPE==ifcRelDefinesByType_TYPE)
		{
			int ifcTypeObject;
			//�ɹ�����ϵȡ�ù������Ķ�����
			sdaiGetAttrBN(ifcRelDefines,"RelatingType",sdaiINSTANCE,&ifcTypeObject); /*IFCEngine API*/

			int *ifcPropertySets,numIfcPropertySets;
			//��ȡ�����Լ����ܼ����
			sdaiGetAttrBN(ifcTypeObject,"HasPropertySets",sdaiAGGR,&ifcPropertySets); /*IFCEngine API*/
			numIfcPropertySets=sdaiGetMemberCount(ifcPropertySets); /*IFCEngine API*/
			int ifcPropertySet; //һ�����Լ��ľ��
			int ifcPropertySet_INSTANCE_TYPE;
			for(int j=0;j<numIfcPropertySets;++j)
			{
				engiGetAggrElement(ifcPropertySets,j,sdaiINSTANCE,&ifcPropertySet); /*IFCEngine API*/
				ifcPropertySet_INSTANCE_TYPE = sdaiGetInstanceType(ifcPropertySet); /*IFCEngine API*/
				if (ifcPropertySet_INSTANCE_TYPE==ifcPropertySet_TYPE) /*IFCEngine API*/
				{
					int *ifcProperties=NULL,numIfcProperties;
					int ifcPropertySet_ComplexProperty_INSTANCE_TYPE=sdaiGetInstanceType(ifcPropertySet); /*IFCEngine API*/
					//ȡ�����Լ���ifcProperties��
					sdaiGetAttrBN(ifcPropertySet, "HasProperties", sdaiAGGR, &ifcProperties); /*IFCEngine API*/
					//ȡ�����Ը���
					numIfcProperties=sdaiGetMemberCount(ifcProperties); /*IFCEngine API*/
					int ifcProperty; //һ�����Եľ��
					int ifcProperty_INSTANCE_TYPE;
					bool bPropertyTypeFound=false;
					for (int k=0;k<numIfcProperties;++k)
					{
						//ȡ������
						engiGetAggrElement(ifcProperties, k, sdaiINSTANCE, &ifcProperty);
						ifcProperty_INSTANCE_TYPE = sdaiGetInstanceType(ifcProperty);

						if	(ifcProperty_INSTANCE_TYPE == ifcComplexProperty_TYPE)
						{
							//	IfcProperty
							//		Name
							//		Description				OPTIONAL
							//		PropertyForDependance	INVERSE
							//		PropertyDependsOn		INVERSE
							//		PartOfComplex			INVERSE
							//	IfcComplexProperty
							//		UsageName
							//		HasProperties

							bPropertyTypeFound = true;
							int thePropertyID=ifcProperty;
							char *thePropertyName;
							//char *thePropertyValue;
							char *thePropertyDescription;
							char *thePropertyUsageName;
							sdaiGetAttrBN(ifcProperty,"Name",sdaiSTRING,&thePropertyName);
							sdaiGetAttrBN(ifcProperty,"Description",sdaiSTRING,&thePropertyDescription);
							sdaiGetAttrBN(ifcProperty,"UsageName",sdaiSTRING,&thePropertyUsageName);
							//�����������ͣ����滹��������Ҫѭ�� TODO
						}

						//�ص���SingleValue���͵�����
						if	(ifcProperty_INSTANCE_TYPE == ifcPropertySingleValue_TYPE)
						{
							//	IfcProperty
							//		Name
							//		Description				OPTIONAL
							//		PropertyForDependance	INVERSE
							//		PropertyDependsOn		INVERSE
							//		PartOfComplex			INVERSE
							//	IfcSimpleProperty
							//	IfcPropertySingleValue
							//		NominalValue
							//		Unit					OPTIONAL
							bPropertyTypeFound = true;
							int thePropertyID=ifcProperty;
							char *thePropertyName;
							char *thePropertyUnit;
							char *thePropertyDescription;
							char *thePropertyNominalValue;
							sdaiGetAttrBN(ifcProperty, "Name", sdaiSTRING, &thePropertyName);
							sdaiGetAttrBN(ifcProperty, "Description", sdaiSTRING, &thePropertyDescription);
							sdaiGetAttrBN(ifcProperty, "NominalValue", sdaiSTRING, &thePropertyNominalValue);
							sdaiGetAttrBN(ifcProperty, "Unit", sdaiSTRING, &thePropertyUnit);
						}

						if	(ifcProperty_INSTANCE_TYPE == ifcPropertyEnumeratedValue_TYPE)
						{
							bPropertyTypeFound = true;
						}

						if	(ifcProperty_INSTANCE_TYPE == ifcPropertyBoundedValue_TYPE)
						{
							bPropertyTypeFound = true;
						}

						if	(ifcProperty_INSTANCE_TYPE == ifcPropertyTableValue_TYPE)
						{
							bPropertyTypeFound = true;
						}

						if	(ifcProperty_INSTANCE_TYPE == ifcPropertyReferenceValue_TYPE)
						{
							bPropertyTypeFound = true;
						}

						if	(ifcProperty_INSTANCE_TYPE == ifcPropertyListValue_TYPE)
						{
							bPropertyTypeFound = true;
						}
					}
				}
			}
		}

		if (ifcInstance_TYPE==ifcRelDefinesByProperties_TYPE)
		{

			int	ifcPropertySet,ifcPropertySet_INSTANCE_TYPE;
			//ȡ��PSet���Լ��ľ��
			sdaiGetAttrBN(ifcRelDefines,"RelatingPropertyDefinition",sdaiINSTANCE,&ifcPropertySet);
			ifcPropertySet_INSTANCE_TYPE = sdaiGetInstanceType(ifcPropertySet);
			if (ifcPropertySet_INSTANCE_TYPE == ifcPropertySet_TYPE)
			{
				TiXmlElement *pPropertySet=new TiXmlElement("PropertySet");
				pPropertySet->SetAttribute("PSetID",ifcPropertySet);

				int *ifcProperties=NULL,numIfcProperties;
				int ifcPropertySet_ComplexProperty_INSTANCE_TYPE=sdaiGetInstanceType(ifcPropertySet); /*IFCEngine API*/
				char *PSetName;
				//ȡ�����Լ���ifcProperties��
				sdaiGetAttrBN(ifcPropertySet,"HasProperties",sdaiAGGR,&ifcProperties); /*IFCEngine API*/
				//ȡ�����Լ�����
				sdaiGetAttrBN(ifcPropertySet,"Name",sdaiSTRING,&PSetName); /*IFCEngine API*/
				//ȡ�����Ը���
				numIfcProperties=sdaiGetMemberCount(ifcProperties); /*IFCEngine API*/

				pPropertySet->SetAttribute("PropertySetName",strParser(PSetName).c_str());
				pPropertySet->SetAttribute("PropertyNumber",numIfcProperties);

				int ifcProperty; //һ�����Եľ��
				int ifcProperty_INSTANCE_TYPE;
				bool bPropertyTypeFound=false;
				for (int k=0;k<numIfcProperties;++k)
				{
					//ȡ������
					engiGetAggrElement(ifcProperties, k, sdaiINSTANCE, &ifcProperty);
					ifcProperty_INSTANCE_TYPE = sdaiGetInstanceType(ifcProperty);

					//�ص���SingleValue���͵�����
					if	(ifcProperty_INSTANCE_TYPE == ifcPropertySingleValue_TYPE)
					{
						//	IfcProperty
						//		Name
						//		Description				OPTIONAL
						//		PropertyForDependance	INVERSE
						//		PropertyDependsOn		INVERSE
						//		PartOfComplex			INVERSE
						//	IfcSimpleProperty
						//	IfcPropertySingleValue
						//		NominalValue
						//		Unit					OPTIONAL //���Ǽ�����λ
						bPropertyTypeFound = true;
						int thePropertyID=ifcProperty;
						char *thePropertyName;
						char *thePropertyUnit;
						char *thePropertyDescription;
						char *thePropertyNominalValue;
						sdaiGetAttrBN(ifcProperty, "Name", sdaiSTRING, &thePropertyName);
						sdaiGetAttrBN(ifcProperty, "Description", sdaiSTRING, &thePropertyDescription);
						sdaiGetAttrBN(ifcProperty, "NominalValue", sdaiSTRING, &thePropertyNominalValue);
						sdaiGetAttrBN(ifcProperty, "Unit", sdaiSTRING, &thePropertyUnit);
						
						/*
						CString Name(strParser(thePropertyName).c_str());
						CString Value(strParser(thePropertyNominalValue).c_str());
						AfxMessageBox(Name+_T("\n")+Value);
						*/
						TiXmlElement *pProperty=new TiXmlElement("Property");
						pProperty->SetAttribute("Name",strParser(thePropertyName).c_str());
						pProperty->SetAttribute("Value",strParser(thePropertyNominalValue).c_str());
						pProperty->SetAttribute("Description",strParser(thePropertyDescription).c_str());

						pPropertySet->LinkEndChild(pProperty);
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcComplexProperty_TYPE)
					{
						//	IfcProperty
						//		Name
						//		Description				OPTIONAL
						//		PropertyForDependance	INVERSE
						//		PropertyDependsOn		INVERSE
						//		PartOfComplex			INVERSE
						//	IfcComplexProperty
						//		UsageName
						//		HasProperties

						bPropertyTypeFound = true;
						int thePropertyID=ifcProperty;
						char *thePropertyName;
						//char *thePropertyValue;
						char *thePropertyDescription;
						char *thePropertyUsageName;
						sdaiGetAttrBN(ifcProperty,"Name",sdaiSTRING,&thePropertyName);
						sdaiGetAttrBN(ifcProperty,"Description",sdaiSTRING,&thePropertyDescription);
						sdaiGetAttrBN(ifcProperty,"UsageName",sdaiSTRING,&thePropertyUsageName);
						//�����������ͣ����滹��������Ҫѭ�� TODO
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcPropertyEnumeratedValue_TYPE)
					{
						bPropertyTypeFound = true;
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcPropertyBoundedValue_TYPE)
					{
						bPropertyTypeFound = true;
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcPropertyTableValue_TYPE)
					{
						bPropertyTypeFound = true;
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcPropertyReferenceValue_TYPE)
					{
						bPropertyTypeFound = true;
					}

					if	(ifcProperty_INSTANCE_TYPE == ifcPropertyListValue_TYPE)
					{
						bPropertyTypeFound = true;
					}
				}

				pRoot->LinkEndChild(pPropertySet);
			}

		}
	}

	XmlPropertyList->LinkEndChild(pRoot);
	return XmlPropertyList;
}

BOOL CIFCProject::SavePropertySet(CString FileName)
{
	//���utf-8�����xml�ļ�
	if (!bOpen)
		return FALSE;

	if (!XmlPropertyList)
		return FALSE;

	TiXmlPrinter printer;
	XmlPropertyList->Accept(&printer);
	const CStringW UnicodeStr(printer.CStr());
	const CStringA UTF8Str=CW2A(UnicodeStr,CP_UTF8);
	CFile theFile;
	theFile.Open(FileName,CFile::modeCreate|CFile::modeWrite);
	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
	theFile.Write(&UTF8BOM,3);
	theFile.Write((LPCSTR)UTF8Str,UTF8Str.GetLength());
	theFile.Close();

	return TRUE;
}

const TiXmlDocument* CIFCProject::GetProperty(int InstanceID)
{
	return NULL;
}

BOOL CIFCProject::SaveProperty(CString FileName)
{
	return TRUE;
}

CString CIFCProject::GetSinglePropertyInPSet(int InstanceID,CString PropertyName)
{
	CString PropertyValue;
	PropertyValue=_T("");

	if (!bOpen)
		return PropertyValue;

	if (IFCGroups.empty())
		return PropertyValue;

	int *ifcRelDefiness,numIfcRelDefiness;
	//ȡ�ù�����ϵ�ļ��ϵľ�����������ĵ�λ�൱��Entity��
	sdaiGetAttrBN(InstanceID,"IsDefinedBy",sdaiAGGR,&ifcRelDefiness); /*IFCEngine API*/
	//ȡ�ù�����ϵ�ĸ���
	numIfcRelDefiness=sdaiGetMemberCount(ifcRelDefiness); /*IFCEngine API*/

	int ifcInstance_TYPE;

	int ifcRelDefines; //һ��������ϵ�ľ����һ��������ϵ��Ӧ��һ��PSet��
	for (int i=0;i<numIfcRelDefiness;++i)
	{
		//ȡ�ù�����ϵ�ľ����ע�⣺��IFC��ϵ����Instance��
		engiGetAggrElement(ifcRelDefiness,i,sdaiINSTANCE,&ifcRelDefines); /*IFCEngine API*/
		//ȡ�ù�����ϵ������
		ifcInstance_TYPE=sdaiGetInstanceType(ifcRelDefines); /*IFCEngine API*/

		if (ifcInstance_TYPE==ifcRelDefinesByProperties_TYPE)
		{
			int	ifcPropertySet,ifcPropertySet_INSTANCE_TYPE;
			//ȡ��PSet���Լ��ľ��
			sdaiGetAttrBN(ifcRelDefines,"RelatingPropertyDefinition",sdaiINSTANCE,&ifcPropertySet);
			ifcPropertySet_INSTANCE_TYPE = sdaiGetInstanceType(ifcPropertySet);
			if (ifcPropertySet_INSTANCE_TYPE == ifcPropertySet_TYPE)
			{
				int *ifcProperties=NULL,numIfcProperties;
				int ifcPropertySet_ComplexProperty_INSTANCE_TYPE=sdaiGetInstanceType(ifcPropertySet); /*IFCEngine API*/
				char *PSetName;
				//ȡ�����Լ���ifcProperties��
				sdaiGetAttrBN(ifcPropertySet,"HasProperties",sdaiAGGR,&ifcProperties); /*IFCEngine API*/
				//ȡ�����Լ�����
				sdaiGetAttrBN(ifcPropertySet,"Name",sdaiSTRING,&PSetName); /*IFCEngine API*/
				//ȡ�����Ը���
				numIfcProperties=sdaiGetMemberCount(ifcProperties); /*IFCEngine API*/

				int ifcProperty; //һ�����Եľ��
				int ifcProperty_INSTANCE_TYPE;
				for (int k=0;k<numIfcProperties;++k)
				{
					//ȡ������
					engiGetAggrElement(ifcProperties, k, sdaiINSTANCE, &ifcProperty);
					ifcProperty_INSTANCE_TYPE = sdaiGetInstanceType(ifcProperty);

					//SingleValue���͵�����
					if	(ifcProperty_INSTANCE_TYPE == ifcPropertySingleValue_TYPE)
					{
						//	IfcProperty
						//		Name
						//		Description				OPTIONAL
						//		PropertyForDependance	INVERSE
						//		PropertyDependsOn		INVERSE
						//		PartOfComplex			INVERSE
						//	IfcSimpleProperty
						//	IfcPropertySingleValue
						//		NominalValue
						//		Unit					OPTIONAL
						int thePropertyID=ifcProperty;
						char *thePropertyName;
						char *thePropertyUnit;
						char *thePropertyDescription;
						char *thePropertyNominalValue;
						//double *thePropertyNominalValue;
						sdaiGetAttrBN(ifcProperty, "Name", sdaiSTRING, &thePropertyName);
						sdaiGetAttrBN(ifcProperty, "Description", sdaiSTRING, &thePropertyDescription);
						sdaiGetAttrBN(ifcProperty, "NominalValue", sdaiSTRING, &thePropertyNominalValue);
						//sdaiGetAttrBN(ifcProperty, "NominalValue", sdaiREAL, &thePropertyNominalValue); //ʵ��
						sdaiGetAttrBN(ifcProperty, "Unit", sdaiSTRING, &thePropertyUnit);

						CString FindingName(strParser(thePropertyName).c_str());
						if (FindingName==PropertyName)
						{
							PropertyValue=CString(strParser(thePropertyNominalValue).c_str());
							/*
							CString returnValue;
							returnValue.Format(_T("%d"),thePropertyNominalValue);
							return returnValue;
							*/
						}
					}
				}
			}
		}
	}

	return PropertyValue;
}

//��PSet���Լ��������ԣ����Ժ�����
BOOL CIFCProject::AddNewPropertyTest(int InstanceID)
{
	if (!bOpen)
		return FALSE;
	if (IFCGroups.empty())
		return FALSE;

	//����ȡ��PSet���Լ������PSet���Լ���GUID������������GUID
	int PSetID=GetInstancePSetID(InstanceID,_T("PSet_Revit_��������"));
	//int PSetID=GetInstancePSetID(InstanceID,_T("PSet_WindowCommon"));
	//���粻���ڣ�����һ��PSet��
	if (PSetID==0)
	{
		return FALSE;
	}

	int *ifcProperties; //���Լ�
	sdaiGetAttrBN(PSetID,"HasProperties",sdaiAGGR,&ifcProperties); /*IFCEngine API*/
	sdaiAppend((int)ifcProperties,sdaiINSTANCE,(void*)BuildPropertySingleValueInstance(_T(""),_T(""),_T("")));

	return TRUE;
}

int CIFCProject::BuildPropertySingleValueInstance(CString name,CString strValue,CString Description)
{
	if (!bOpen)
		return 0;

	int ifcPropertySingleValueInstance;
	ifcPropertySingleValueInstance=sdaiCreateInstanceBN(ProjectID,"IFCPROPERTYSINGLEVALUE");
	int OID=engiGetInstanceLocalId(ifcPropertySingleValueInstance);

	sdaiPutAttrBN(ifcPropertySingleValueInstance,"Name",sdaiSTRING,"C_Carbon_TEST");
	sdaiPutAttrBN(ifcPropertySingleValueInstance,"Description",sdaiSTRING,"for test");
	void *nominalValueADB;
	nominalValueADB = sdaiCreateADB(sdaiSTRING, "1kg");
	sdaiPutADBTypePath(nominalValueADB,1,"IFCTEXT");
	sdaiPutAttrBN(ifcPropertySingleValueInstance, "NominalValue", sdaiADB, (void*) nominalValueADB);

	return ifcPropertySingleValueInstance;
}

int CIFCProject::GetInstancePSetID(int InstanceID,CString PSetName)
{
	if (!bOpen)
		return 0;
	if (IFCGroups.empty())
		return 0;

	int ifcPropertySet=0;

	int *ifcRelDefiness,numIfcRelDefiness;
	//ȡ�ù�����ϵ�ļ��ϵľ�����������ĵ�λ�൱��Entity��
	sdaiGetAttrBN(InstanceID,"IsDefinedBy",sdaiAGGR,&ifcRelDefiness); /*IFCEngine API*/
	//ȡ�ù�����ϵ�ĸ���
	numIfcRelDefiness=sdaiGetMemberCount(ifcRelDefiness); /*IFCEngine API*/

	int ifcInstance_TYPE;

	int ifcRelDefines; //һ��������ϵ�ľ����һ��������ϵ��Ӧ��һ��PSet��
	for (int i=0;i<numIfcRelDefiness;++i)
	{
		//ȡ�ù�����ϵ�ľ����ע�⣺��IFC��ϵ����Instance��
		engiGetAggrElement(ifcRelDefiness,i,sdaiINSTANCE,&ifcRelDefines); /*IFCEngine API*/
		//ȡ�ù�����ϵ������
		ifcInstance_TYPE=sdaiGetInstanceType(ifcRelDefines); /*IFCEngine API*/
		if (ifcInstance_TYPE==ifcRelDefinesByProperties_TYPE)
		{
			int	ifcPropertySet_INSTANCE_TYPE;
			//ȡ��PSet���Լ��ľ��
			sdaiGetAttrBN(ifcRelDefines,"RelatingPropertyDefinition",sdaiINSTANCE,&ifcPropertySet);
			ifcPropertySet_INSTANCE_TYPE = sdaiGetInstanceType(ifcPropertySet);
			if (ifcPropertySet_INSTANCE_TYPE == ifcPropertySet_TYPE)
			{
				char *pPSetName;
				//ȡ�����Լ�����
				sdaiGetAttrBN(ifcPropertySet,"Name",sdaiSTRING,&pPSetName); /*IFCEngine API*/
				if (PSetName.CompareNoCase(CString(strParser(pPSetName).c_str()))==0)
				{
					return ifcPropertySet;
				}
			}
		}
	}

	return 0;
}

/************************************************************************/
/*    IFCʵ����ʵ��                                                     */
/************************************************************************/
//����
CIFCEntity::CIFCEntity()
{
	id=NULL;
	groupID=0;
	name="";
	name_cn="";
	name_ifc="";
	num=0;
	numSelected=0;
	EntityInstances.clear();
}

CIFCEntity::CIFCEntity(CString ifcEntityName,CString EntityName,CString EntityNameCN,int *pID,int GroupID)
{
	id=pID;
	groupID=GroupID;
	name=EntityName;
	name_cn=EntityNameCN;
	name_ifc=ifcEntityName;
	num=0;
	numSelected=0;
	EntityInstances.clear();
}

//����
CIFCEntity::~CIFCEntity()
{
	id=NULL;
	EntityInstances.clear();
}

void CIFCEntity::SetEntity(CString ifcEntityName, CString EntityName, CString EntityNameCN, int *pID, int GroupID)
{
	id=pID;
	groupID=GroupID;
	name=EntityName;
	name_cn=EntityNameCN;
	name_ifc=ifcEntityName;
	num=0;
	numSelected=0;
	EntityInstances.clear();
}

//��ʼ��
BOOL CIFCEntity::Initialize(int ProjectID,int ModelType)
{
	if (id==NULL)
		return FALSE;

	//��ȡʵ��������
	//ʵ���ʵ��������API��������
	num = sdaiGetMemberCount(id); /*IFCEngine API*/
	numSelected = num;

	if (num<=0)
		return FALSE;

	int  InstanceID;    //ָ��ʵ��������
	char *GloalID;      //GloalID
	char *InstanceName; //
	for (int i = 0; i < num; ++i) 
	{
		engiGetAggrElement(id, i, sdaiINSTANCE, &InstanceID);                /*IFCEngine API*/ //ȡ��ʵ��ID
		sdaiGetAttrBN(InstanceID, "GlobalId", sdaiSTRING, &GloalID);         /*IFCEngine API*/ //ȡ��ʵ��GlobalID
		sdaiGetAttrBN(InstanceID, "Name", sdaiSTRING, &InstanceName);        /*IFCEngine API*/ //ȡ��ʵ��Name
		int OID=engiGetInstanceLocalId(InstanceID);                          /*IFCEngine API*/ //ȡ��ʵ���к�
		//sdaiGetAttrBN(InstanceID, "Description", sdaiSTRING, &Description);  /*IFCEngine API*/ //ȡ��ʵ��Description
		//sdaiGetAttrBN(InstanceID, "Object Type", sdaiSTRING, &ObjectType);   /*IFCEngine API*/ //ȡ��ʵ��Object Type
		//sdaiGetAttrBN(InstanceID, "Storey Name", sdaiSTRING, &StoreyName);   /*IFCEngine API*/ //ȡ��ʵ��������¥��
		//sdaiGetAttrBN(InstanceID, "Position", sdaiSTRING, &Position);        /*IFCEngine API*/ //ȡ��ʵ�����õ�����
		string instanceName=strParser(InstanceName);                  //�����ַ���
		/*
#ifdef UNICODE
		TCHAR *szString;
		int size=strlen(string);
		szString = new TCHAR[size+1];
		MultiByteToWideChar(CP_ACP,0,string,-1,szString,size);
		szString[size]='\0';
#else
		char *szString;
		int size=strlen(string);
		szString=new char[size+1];
		strcpy(szString,string);
		szString[size]='\0';
#endif
		*/
		CString szGloalID(GloalID);
		CString szInstanceName(instanceName.c_str());

		//����ʵ��
		CIFCInstance ifcInstance(InstanceID,szGloalID,OID,szInstanceName);
		//��ʼ��ʵ��
		ifcInstance.Initialize(ProjectID,ModelType);
		//����
		EntityInstances.insert(Instance_Pair(InstanceID,ifcInstance));
	}

	return TRUE;
}

int CIFCEntity::GetNumber()
{
	return num;
}

int CIFCEntity::GetNumSelected(BOOL reMake)
{
	if (reMake)
	{
		numSelected=0;
		Instance_constIter iter=EntityInstances.begin();
		for (;iter!=EntityInstances.end();++iter)
		{
			CIFCInstance instance=iter->second;
			if (instance.GetSelected())
				numSelected++;
		}
	}

	return numSelected;
}

BOOL CIFCEntity::SetAllSelected(BOOL bInstancesSelected)
{
	if ((num<=0)||EntityInstances.empty())
		return FALSE;

	Instance_Iter instanceIter;
	Instance_Pair instancePair;
	instanceIter=EntityInstances.begin();
	for (;instanceIter!=EntityInstances.end();++instanceIter)
	{
		int instanceID=instanceIter->first;
		CIFCInstance instance=instanceIter->second;
		instance.SetSelected(bInstancesSelected);
		EntityInstances[instanceID]=instance;
	}

	if (bInstancesSelected==TRUE)
		numSelected=num;
	else
		numSelected=0;

	return TRUE;
}

CString CIFCEntity::GetEntityName()
{
	return name;
}

CIFCInstance CIFCEntity::GetInstance(int InstanceID)
{
	if ((num<=0)||EntityInstances.empty())
	{
		CIFCInstance NullInstance;
		return NullInstance;
	}

	return EntityInstances[InstanceID];
}


BOOL CIFCEntity::SetInstance(int InstanceID, CIFCInstance ifcInstance)
{
	if (InstanceID<=0)
		return FALSE;

	if (InstanceID!=ifcInstance.GetID())
		return FALSE;

	EntityInstances[InstanceID]=ifcInstance;
	ReMakeNumber();

	return TRUE;
}

BOOL CIFCEntity::SetInstanceSelected(int InstanceID, BOOL viewable)
{
	if (InstanceID<=0)
		return FALSE;

	CIFCInstance instance=EntityInstances[InstanceID];
	if (instance.GetID()==0) //������
	{
		EntityInstances.erase(InstanceID);
		return FALSE;
	}

	instance.SetSelected(viewable);
	EntityInstances[InstanceID]=instance;
	ReMakeNumber();

	return TRUE;
}

const InstanceGroup* CIFCEntity::GetInstanceGroup()
{
	if (EntityInstances.empty())
		return NULL;

	return &EntityInstances;
}

CString CIFCEntity::GetNameCN()
{
	return name_cn;
}

CString CIFCEntity::GetNameIFC()
{
	return name_ifc;
}

int CIFCEntity::GetGroupID()
{
	return groupID;
}

const CIFCInstance* CIFCEntity::GetIFCInstance(int InstanceID)
{
	return NULL;
}

int CIFCEntity::stat()
{
	if (0==numSelected)
		return UNCHECKED;
	else if(num==numSelected)
		return ALLCHECKED;
	else
		return PARTLYCHECKED;
}

void CIFCEntity::ReMakeNumber()
{
	num=(int)EntityInstances.size();
	numSelected=0;
	if (num==0)
	{
		return;
	}

	Instance_constIter iter=EntityInstances.begin();
	for (;iter!=EntityInstances.end();iter++)
	{
		CIFCInstance instance=iter->second;
		if (instance.GetSelected())
			numSelected++;
	}
}

/************************************************************************/
/*    IFCʵ����ʵ��                                                     */
/************************************************************************/
//����
CIFCInstance::CIFCInstance()
{
	id=0;
	GlobalID="";
	bSelected=TRUE;
	Name="";
	OID=0;
	GlobalID_readable="";
	Description="";
	ObjectType="";

	startVertex=0;
	startIndex=0;
	primitiveCount=0;
}

CIFCInstance::CIFCInstance(int InstanceID, CString GID, int OId, CString InstanceName)
{
	id=InstanceID;
	GlobalID=GID;
	bSelected=TRUE;
	Name=InstanceName;
	OID=OId;
	GlobalID_readable="";
	Description="";
	ObjectType="";

	startVertex=0;
	startIndex=0;
	primitiveCount=0;
}

BOOL CIFCInstance::Initialize(int ProjectID,int ModelType)
{
	if (ProjectID<=0)
		return FALSE;

	//��ȡÿ��ʵ���Ķ�����ʼָ��
	//������������3ά��ʾģʽ
	getInstanceInModelling(ProjectID, id, ModelType, &startVertex, &startIndex, &primitiveCount); /*IFCEngine API*/
	return TRUE;
}

//����
CIFCInstance::~CIFCInstance()
{

}
int CIFCInstance::GetID()
{
	return id;
}

CString CIFCInstance::GetGlobalID()
{
	return GlobalID;
}

int CIFCInstance::GetOID()
{
	return OID;
}

BOOL CIFCInstance::GetSelected()
{
	return bSelected;
}

void CIFCInstance::SetSelected(BOOL bSelecting)
{
	bSelected=bSelecting;
}

int CIFCInstance::GetStartVertex()
{
	return startVertex;
}

int CIFCInstance::GetStartIndex()
{
	return startIndex;
}

int CIFCInstance::GetPrimitiveCount()
{
	return primitiveCount;
}

CString CIFCInstance::GetName()
{
	return Name;
}

/************************************************************************/
/*    IFC������ʵ��                                                     */
/************************************************************************/
//����
CIFCProperty::CIFCProperty()
{
	propertyId=0;
	m_name=_T("");
	m_value=_T("");
}

CIFCProperty::CIFCProperty(int PropertyID,CString Name,CString Value,CString Description)
{
	propertyId=PropertyID;
	m_name=Name;
	m_value=Value;
	m_description=Description;
}

//����
CIFCProperty::~CIFCProperty()
{

}