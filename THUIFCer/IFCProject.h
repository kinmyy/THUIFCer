//ʹ��IFCEngine�ṩ��API������һ����װIFC�������

#pragma once

#include <map>
#include "IFCEngine.h"
#include "define.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

typedef struct CUSTOMVERTEX {
	float	x;
	float	y;
	float	z;
	float	nx;
	float	ny;
	float	nz;
	float   tu;
	float   tv;
}	customvertex;

class CIFCProject;
class CIFCEntity;
class CIFCInstance;
class CIFCProperty;

typedef std::map<CString,CIFCEntity> EntityGroup;
typedef std::map<CString,CIFCEntity>::iterator Entities_Iter;
typedef std::map<CString,CIFCEntity>::const_iterator Entities_constIter;
typedef std::pair<CString,CIFCEntity> Entities_Pair;
typedef std::map<int,CIFCInstance> InstanceGroup;
typedef std::map<int,CIFCInstance>::iterator Instance_Iter;
typedef std::map<int,CIFCInstance>::const_iterator Instance_constIter;
typedef std::pair<int,CIFCInstance> Instance_Pair;
typedef std::map<int,CString> EntityMap;
typedef std::map<int,CString>::iterator EntityMap_Iter;
typedef std::map<int,CString>::const_iterator EntityMap_constIter;
typedef std::pair<int,CString> EntityMap_Pair;

/************************************************************************/
/*    IFC������                                                         */
/************************************************************************/
class CIFCProject
{
private:
	CString     FilePath;                         //IFCģ���ļ�·��
	CString     FileName;                         //IFCģ���ļ���
	int         ProjectID;                        //IFCEngine��ģ�ͷ��صı�ʶģ�͵ľ��
	CString     IFCSchemaVersion;                 //IFCģ�͵İ汾����IFC�ļ�����
	CString     IFCSchemaName;                    //Schema��·��
	CString     ProjectName;                      //��������
	CString     GlobalId;                         //���̵�GlobalID
	CString     Owner;                            //������
	BOOL        bOpen;                            //�Ƿ��Ѿ��򿪹���
	EntityGroup IFCGroups;                        //��IFCʵ����飬����Ϊʵ�����ƣ�CIFCEntity.name��
	EntityMap   IFCGroupsMap;                     //ʵ�����ͷ���IDһһ��Ӧ��ӳ���
	int         CurrentGroupID;                   //����ţ��ڸ���һ��ʵ������1
	int         numVertices;                      //����������Ԫ�ظ���
	int         numIndices;                       //���㻺��Ԫ�ظ���
	float	    *pVertices;                       //����������
	int         *pIndices;                        //���㻺��
	int         ModelType;                        //��άģ�͵���ʾģʽ
	TiXmlDocument *XmlEntityTree;                 //��ʵ������xml�ṹ���ĵ�ָ��
	TiXmlDocument *XmlFloorTree;                  //��¥����֯��xml�ṹ���ĵ�ָ��
	TiXmlDocument *XmlPropertyList;               //ʵ���PSet���Լ���xml�ṹ���ĵ�ָ��

	int         ifcRelDefinesByType_TYPE;
	int         ifcRelDefinesByProperties_TYPE;
	int         ifcPropertySet_TYPE;
	int         ifcComplexProperty_TYPE;
	int         ifcPropertySingleValue_TYPE;
	int         ifcPropertyEnumeratedValue_TYPE;
	int         ifcPropertyBoundedValue_TYPE;
	int         ifcPropertyTableValue_TYPE;
	int         ifcPropertyReferenceValue_TYPE;
	int         ifcPropertyListValue_TYPE;

private:
	BOOL RetrieveEntities(CString ObjectSPFFName,CString ObjectGroupName,CString ObjectGroupNameCN); //��ȡʵ����Ϣ
	BOOL Initialize();                                                          //��ʼ��
	void InitializeIfcType();                                                   //��ʼ��һЩ�õ���IFC��������
	int  MakeID();                                                              //����һ������ID��CIFCEntity���飩

public:
	CIFCProject();
	CIFCProject(CString filePath,CString ifcSchemaName);
	~CIFCProject();
	BOOL OpenIFCProject();                                                      //��IFC����
	BOOL OpenIFCProject(CString filePath,CString ifcSchemaName);                //��IFC����
	BOOL CopyIFCProject(int project);                                           //����һ���Ѿ��򿪵Ĺ���
	BOOL CloseIFCProject();                                                     //�ر�IFC����
	BOOL CreateIFCProject(CString filePath,CString ifcSchemaName);              //����һ���հ׹���
	BOOL SaveIFCProject();                                                      //����IFC���̣�Ĭ��·��
	BOOL SaveIFCProject(CString filePath);                                      //����IFC���̣�����·������
	BOOL OpenFlag();                                                            //�����Ƿ��Ѿ��ɹ���
	CString GetIFCFilePath();                                                   //��ȡIFC�ļ���·��
	CString GetIFCSchemaPath();                                                 //��ȡSchema�ļ���·��
	BOOL SetPath(CString filePath,CString ifcSchemaName);                       //����IFCģ���ļ���·����Schema��·����������ģ���Ѿ��ر�ʱ��������
	int GetProjectID();                                                         //��ȡ���̵ľ��
	CIFCEntity GetEntity(CString EntityName);                                   //��ȡIFC Entity��ͨ��ʵ�����ƣ�
	CIFCEntity GetEntity(int GroupID);                                          //��ȡIFC Entity��ͨ������ID��
	const CIFCEntity *GetIFCEntity(CString GroupName);                          //��ȡIFC Entity�ĳ�ָ�루ͨ��ʵ�����ƣ�
	const CIFCEntity *GetIFCEntity(int GroupID);                                //��ȡIFC Entity�ĳ�ָ�루ͨ������ID��
	BOOL SetEntity(CString EntityName, CIFCEntity ifcEntity);                   //д��IFC Entity
	BOOL SetEntitySelected(CString EntityName, BOOL viewable);                  //����IFC Entity�ɼ��ԣ�һ�鹹������ͨ��ʵ������
	BOOL SetEntitySelected(int GroupID, BOOL viewable);                         //����IFC Entity�ɼ��ԣ�һ�鹹������ͨ������ID
	BOOL SetAllSelected(BOOL viewalbe);                                         //��������ʵ���ɼ���
	BOOL SetInstanceSelected(CString EntityName,int InstanceID, BOOL viewalbe); //����ָ��ʵ���ɼ��ԣ�ͨ������ʵ�������
	BOOL SetInstanceSelected(int GroupID, int InstanceID, BOOL viewalbe);       //����ָ��ʵ���ɼ��ԣ�ͨ������ʵ��ķ���ID
	int *GetEntityID(CString EntityName);                                       //��ȡIFC Entity��ָ�룬ͨ��ʵ�����ƣ���ָ��ָ��IFC Engine�򿪹��̺���ڲ����ݣ�Ӧ����Ϊ˽�к�����
	int *GetEntityID(int GroupID);                                              //��ȡIFC Entity��ָ�룬ͨ������ID����ָ��ָ��IFC Engine�򿪹��̺���ڲ����ݣ�Ӧ����Ϊ˽�к�����
	int GetInstanceNumByAPI(int *groupID);                                      //��ȡʵ����������ͨ��IFC Entity��ָ�루ֱ�ӵ���IFC Engine APIȡ�ã�Ӧ����Ϊ˽�к�����
	int GetInstanceNumByAPI(CString EntityName);                                //��ȡʵ����������ͨ��ʵ������ƣ�����IFC Engine APIȡ�ã�Ӧ����Ϊ˽�к�����
	int GetInstanceNum(CString EntityName);                                     //��ȡʵ����������ͨ��ʵ�������
	int GetInstanceNum(int GroupID);                                            //��ȡʵ����������ͨ������ID
	int GetInstanceNumSelected(CString EntityName);                             //��ȡʵ���ı�ѡ�е�������ͨ��ʵ�������
	int GetInstanceNumSelected(int GroupID);                                    //��ȡʵ���ı�ѡ�е�������ͨ������ID
	void SetModelType(int modeltype);                                           //������ʾģʽ
	int GetModelType();                                                         //��ȡ��ʾģʽ
	int GetNumVertices();                                                       //
	int GetNumIndices();                                                        //
	const float *GetpVertices();                                                //
	const int *GetpIndices();                                                   //
	const EntityGroup *GetEntityGroup();                                        //����ָ��ʵ�弯�ϵĳ�ָ�루std:map��
	const TiXmlDocument *GetEntityTree(BOOL reMake);                            //������IFC Entity��������νṹ��xml�ĵ�������ָ��
	const TiXmlDocument *GetFloorTree(BOOL reMake);                             //������¥��Ϊ��ε����νṹ��xml�ĵ�������ָ��
	BOOL SaveEnityTree(CString FileName,BOOL reMake=TRUE);                      //������IFC Entity��������νṹ��xml�ĵ�
	BOOL SaveFloorTree(CString FileName,BOOL reMake=TRUE);                      //������¥��Ϊ��ε����νṹ��xml�ĵ�
	void OutputUTF8XmlTest();                                                   //���utf-8 xml�ĵ����ԣ������ú�����
	CString FindEntityName(int GroupID);                                        //ͨ������Ų���IFC Entity������
	const TiXmlDocument *GetPropertySet(int InstanceID);                        //����ʵ����PSet���Լ��ϵ�xml�ĵ�ָ��
	BOOL SavePropertySet(CString FileName);                                     //����PSet����Ϊxml�ĵ�
	const TiXmlDocument *GetProperty(int InstanceID);                           //����ʵ�������ԣ�Դ��IFC�̳���ϵ�����ϵ�xml�ĵ�ָ��
	BOOL SaveProperty(CString FileName);                                        //����ʵ�������ԣ�Դ��IFC�̳���ϵ������Ϊxml�ĵ�
	CString GetSinglePropertyInPSet(int InstanceID,CString PropertyName);       //����PSet���Լ��еĵ�������
	int GetInstancePSetID(int InstanceID,CString PSetName);                     //ȡ��ʵ��PSet���Լ��ľ��
	BOOL AddNewPropertyTest(int InstanceID);                                    //��PSet����Ӳ������ԣ������ú�����
	int BuildPropertySingleValueInstance(CString name,CString strValue,CString Description); //�ڹ������½�һ����ֵ����
};


/************************************************************************/
/*    IFCʵ����                                                         */
/************************************************************************/
class CIFCEntity
{
	friend class CIFCProject;

private:
	int         numSelected;                     //��ѡ�У��ɼ�����ʵ������
	int         *id;                             //������һ��ʵ��ľ��
	int         groupID;                         //��CIFCProject���ɵ�GroupID
	CString     name;                            //ʵ������ƣ�Ӣ�ģ�
	CString     name_cn;                         //ʵ������ƣ����ģ�
	CString     name_ifc;                        //ʵ����IFC�е�����
	int         num;                             //ʵ������
	InstanceGroup EntityInstances;               //ʵ��

public:
	CIFCEntity();
	CIFCEntity(CString ifcEntityName,CString EntityName,CString EntityNameCN,int *pID,int GroupID);
	~CIFCEntity();
	BOOL Initialize(int ProjectID,int ModelType);                                                //��ʼ��
	int GetNumber();                                                                             //��ȡʵ������
	int GetNumSelected(BOOL reMake=FALSE);                                                       //��ȡ��ѡ�У��ɼ�����ʵ������
	BOOL SetAllSelected(BOOL bInstancesSelected);                                                //���ÿɼ��ԣ�ȫ��ʵ����
	CString GetEntityName();                                                                     //��ȡʵ������
	void SetEntity(CString ifcEntityName,CString EntityName,CString EntityNameCN,int *pID,int GroupID);  //����ʵ�壨��ʼ��һ���ն������ã�
	CIFCInstance GetInstance(int InstanceID);                                                    //��ȡʵ��
	BOOL SetInstance(int InstanceID, CIFCInstance ifcInstance);                                  //�޸�ʵ������������ڻ����һ����ʵ��
	BOOL SetInstanceSelected(int InstanceID, BOOL viewable);                                     //���õ���ʵ���ɼ���
	const InstanceGroup* GetInstanceGroup();                                                     //����ָ��ʵ�����ϵĳ�ָ�루std:map��
	CString GetNameCN();                                                                         //����ʵ�����������
	CString GetNameIFC();                                                                        //����ʵ���IFC����
	int GetGroupID();                                                                            //����ʵ��ķ���ID
	const CIFCInstance* GetIFCInstance(int InstanceID);                                          //����ָ��һ��ʵ���ĳ�ָ��
	int stat();                                                                                  //����ĵ�ǰ״̬��ȫ����ѡ�У����ֱ�ѡ�У�ȫ��ѡ�У�
	void ReMakeNumber();                                                                         //���¼��㱻ѡ��ʵ���ĸ���
};


/************************************************************************/
/*    IFCʵ����                                                         */
/************************************************************************/
class CIFCInstance
{
	friend class CIFCProject;
	friend class CIFCEntity;

public:
	BOOL         bSelected;           //�Ƿ�ѡ��
	int          id;                  //IFCEngine���صı�ʶʵ��ľ��
	CString      GlobalID;            //ȫ��ΨһID��GUID��
	int          OID;                 //�����ļ��д���ʵ�����к�
	CString      GlobalID_readable;   //16���ƻ���GUID
	CString      Name;                //����
	CString      Description;         //����
	CString      ObjectType;          //����

	//std::map<int,CIFCProperty> Properties; //���Լ�

	int          startVertex;         //��ʼ����
	int          startIndex;          //
	int          primitiveCount;      //�������

public:
	CIFCInstance();
	CIFCInstance(int InstanceID,CString GID,int OId,CString InstanceName);
	~CIFCInstance();
	BOOL Initialize(int ProjectID,int ModelType);
	BOOL GetSelected();                //�����Ƿ�ɼ�
	void SetSelected(BOOL bSelecting); //���ÿɼ���
	int  GetID();                      //����ʵ��ID
	CString GetGlobalID();             //����ȫ��ΨһID
	int GetOID();                      //���������ļ��д���ʵ�����к�
	int GetStartVertex();              //
	int GetStartIndex();               //
	int GetPrimitiveCount();           //
	CString GetName();                 //����ʵ������
};


/************************************************************************/
/*    IFC�����ࣨ��������)                                              */
/************************************************************************/
class CIFCProperty
{
private:
	int propertyId;
	CString m_name;
	CString m_value;
	CString m_description;

public:
	CIFCProperty();
	CIFCProperty(int PropertyID,CString Name,CString Value,CString Description);
	~CIFCProperty();
};
