//使用IFCEngine提供的API函数进一步封装IFC抽象对象

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
/*    IFC工程类                                                         */
/************************************************************************/
class CIFCProject
{
private:
	CString     FilePath;                         //IFC模型文件路径
	CString     FileName;                         //IFC模型文件名
	int         ProjectID;                        //IFCEngine打开模型返回的标识模型的句柄
	CString     IFCSchemaVersion;                 //IFC模型的版本，由IFC文件读出
	CString     IFCSchemaName;                    //Schema的路径
	CString     ProjectName;                      //工程名称
	CString     GlobalId;                         //工程的GlobalID
	CString     Owner;                            //所有者
	BOOL        bOpen;                            //是否已经打开工程
	EntityGroup IFCGroups;                        //以IFC实体分组，主键为实体名称（CIFCEntity.name）
	EntityMap   IFCGroupsMap;                     //实体分组和分组ID一一对应的映射表
	int         CurrentGroupID;                   //分组号，在赋给一组实体后递增1
	int         numVertices;                      //顶点数据流元素个数
	int         numIndices;                       //顶点缓存元素个数
	float	    *pVertices;                       //顶点数据流
	int         *pIndices;                        //顶点缓存
	int         ModelType;                        //三维模型的显示模式
	TiXmlDocument *XmlEntityTree;                 //以实体分组的xml结构化文档指针
	TiXmlDocument *XmlFloorTree;                  //以楼层组织的xml结构化文档指针
	TiXmlDocument *XmlPropertyList;               //实体的PSet属性集的xml结构化文档指针

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
	BOOL RetrieveEntities(CString ObjectSPFFName,CString ObjectGroupName,CString ObjectGroupNameCN); //获取实体信息
	BOOL Initialize();                                                          //初始化
	void InitializeIfcType();                                                   //初始化一些用到的IFC数据类型
	int  MakeID();                                                              //产生一个分组ID（CIFCEntity分组）

public:
	CIFCProject();
	CIFCProject(CString filePath,CString ifcSchemaName);
	~CIFCProject();
	BOOL OpenIFCProject();                                                      //打开IFC工程
	BOOL OpenIFCProject(CString filePath,CString ifcSchemaName);                //打开IFC工程
	BOOL CopyIFCProject(int project);                                           //拷贝一个已经打开的工程
	BOOL CloseIFCProject();                                                     //关闭IFC工程
	BOOL CreateIFCProject(CString filePath,CString ifcSchemaName);              //创建一个空白工程
	BOOL SaveIFCProject();                                                      //保存IFC工程，默认路径
	BOOL SaveIFCProject(CString filePath);                                      //保存IFC工程，传入路径参数
	BOOL OpenFlag();                                                            //工程是否已经成功打开
	CString GetIFCFilePath();                                                   //获取IFC文件的路径
	CString GetIFCSchemaPath();                                                 //获取Schema文件的路径
	BOOL SetPath(CString filePath,CString ifcSchemaName);                       //设置IFC模型文件的路径和Schema的路径，必须在模型已经关闭时才能设置
	int GetProjectID();                                                         //获取工程的句柄
	CIFCEntity GetEntity(CString EntityName);                                   //获取IFC Entity（通过实体名称）
	CIFCEntity GetEntity(int GroupID);                                          //获取IFC Entity（通过分组ID）
	const CIFCEntity *GetIFCEntity(CString GroupName);                          //获取IFC Entity的常指针（通过实体名称）
	const CIFCEntity *GetIFCEntity(int GroupID);                                //获取IFC Entity的常指针（通过分组ID）
	BOOL SetEntity(CString EntityName, CIFCEntity ifcEntity);                   //写入IFC Entity
	BOOL SetEntitySelected(CString EntityName, BOOL viewable);                  //设置IFC Entity可见性（一组构件），通过实体名称
	BOOL SetEntitySelected(int GroupID, BOOL viewable);                         //设置IFC Entity可见性（一组构件），通过分组ID
	BOOL SetAllSelected(BOOL viewalbe);                                         //设置所有实例可见性
	BOOL SetInstanceSelected(CString EntityName,int InstanceID, BOOL viewalbe); //设置指定实例可见性，通过所属实体的名称
	BOOL SetInstanceSelected(int GroupID, int InstanceID, BOOL viewalbe);       //设置指定实例可见性，通过所属实体的分组ID
	int *GetEntityID(CString EntityName);                                       //获取IFC Entity的指针，通过实体名称（此指针指向IFC Engine打开工程后的内部数据，应调整为私有函数）
	int *GetEntityID(int GroupID);                                              //获取IFC Entity的指针，通过分组ID（此指针指向IFC Engine打开工程后的内部数据，应调整为私有函数）
	int GetInstanceNumByAPI(int *groupID);                                      //获取实例的数量，通过IFC Entity的指针（直接调用IFC Engine API取得，应调整为私有函数）
	int GetInstanceNumByAPI(CString EntityName);                                //获取实例的数量，通过实体的名称（调用IFC Engine API取得，应调整为私有函数）
	int GetInstanceNum(CString EntityName);                                     //获取实例的数量，通过实体的名称
	int GetInstanceNum(int GroupID);                                            //获取实例的数量，通过分组ID
	int GetInstanceNumSelected(CString EntityName);                             //获取实例的被选中的数量，通过实体的名称
	int GetInstanceNumSelected(int GroupID);                                    //获取实例的被选中的数量，通过分组ID
	void SetModelType(int modeltype);                                           //设置显示模式
	int GetModelType();                                                         //获取显示模式
	int GetNumVertices();                                                       //
	int GetNumIndices();                                                        //
	const float *GetpVertices();                                                //
	const int *GetpIndices();                                                   //
	const EntityGroup *GetEntityGroup();                                        //返回指向实体集合的常指针（std:map）
	const TiXmlDocument *GetEntityTree(BOOL reMake);                            //生成以IFC Entity分组的树形结构的xml文档并返回指针
	const TiXmlDocument *GetFloorTree(BOOL reMake);                             //生成以楼层为层次的树形结构的xml文档并返回指针
	BOOL SaveEnityTree(CString FileName,BOOL reMake=TRUE);                      //保存以IFC Entity分组的树形结构的xml文档
	BOOL SaveFloorTree(CString FileName,BOOL reMake=TRUE);                      //保存以楼层为层次的树形结构的xml文档
	void OutputUTF8XmlTest();                                                   //输出utf-8 xml文档测试（测试用函数）
	CString FindEntityName(int GroupID);                                        //通过分组号查找IFC Entity的名称
	const TiXmlDocument *GetPropertySet(int InstanceID);                        //返回实例的PSet属性集合的xml文档指针
	BOOL SavePropertySet(CString FileName);                                     //保存PSet属性为xml文档
	const TiXmlDocument *GetProperty(int InstanceID);                           //返回实例的属性（源自IFC继承体系）集合的xml文档指针
	BOOL SaveProperty(CString FileName);                                        //保存实例的属性（源自IFC继承体系）集合为xml文档
	CString GetSinglePropertyInPSet(int InstanceID,CString PropertyName);       //返回PSet属性集中的单个属性
	int GetInstancePSetID(int InstanceID,CString PSetName);                     //取得实例PSet属性集的句柄
	BOOL AddNewPropertyTest(int InstanceID);                                    //向PSet集添加测试属性（测试用函数）
	int BuildPropertySingleValueInstance(CString name,CString strValue,CString Description); //在工程内新建一个单值属性
};


/************************************************************************/
/*    IFC实体类                                                         */
/************************************************************************/
class CIFCEntity
{
	friend class CIFCProject;

private:
	int         numSelected;                     //被选中（可见）的实例个数
	int         *id;                             //代表这一类实体的句柄
	int         groupID;                         //由CIFCProject生成的GroupID
	CString     name;                            //实体的名称（英文）
	CString     name_cn;                         //实体的名称（中文）
	CString     name_ifc;                        //实体在IFC中的名称
	int         num;                             //实例个数
	InstanceGroup EntityInstances;               //实例

public:
	CIFCEntity();
	CIFCEntity(CString ifcEntityName,CString EntityName,CString EntityNameCN,int *pID,int GroupID);
	~CIFCEntity();
	BOOL Initialize(int ProjectID,int ModelType);                                                //初始化
	int GetNumber();                                                                             //获取实例个数
	int GetNumSelected(BOOL reMake=FALSE);                                                       //获取被选中（可见）的实例个数
	BOOL SetAllSelected(BOOL bInstancesSelected);                                                //设置可见性（全体实例）
	CString GetEntityName();                                                                     //获取实体名称
	void SetEntity(CString ifcEntityName,CString EntityName,CString EntityNameCN,int *pID,int GroupID);  //设置实体（初始化一个空对象后调用）
	CIFCInstance GetInstance(int InstanceID);                                                    //获取实例
	BOOL SetInstance(int InstanceID, CIFCInstance ifcInstance);                                  //修改实例，如果不存在会插入一个新实例
	BOOL SetInstanceSelected(int InstanceID, BOOL viewable);                                     //设置单个实例可见性
	const InstanceGroup* GetInstanceGroup();                                                     //返回指向实例集合的常指针（std:map）
	CString GetNameCN();                                                                         //返回实体的中文名称
	CString GetNameIFC();                                                                        //返回实体的IFC名称
	int GetGroupID();                                                                            //返回实体的分组ID
	const CIFCInstance* GetIFCInstance(int InstanceID);                                          //返回指向一个实例的常指针
	int stat();                                                                                  //分组的当前状态（全部被选中，部分被选中，全不选中）
	void ReMakeNumber();                                                                         //重新计算被选中实例的个数
};


/************************************************************************/
/*    IFC实例类                                                         */
/************************************************************************/
class CIFCInstance
{
	friend class CIFCProject;
	friend class CIFCEntity;

public:
	BOOL         bSelected;           //是否被选中
	int          id;                  //IFCEngine返回的标识实体的句柄
	CString      GlobalID;            //全局唯一ID（GUID）
	int          OID;                 //物理文件中代表实例的行号
	CString      GlobalID_readable;   //16进制化的GUID
	CString      Name;                //名称
	CString      Description;         //描述
	CString      ObjectType;          //类型

	//std::map<int,CIFCProperty> Properties; //属性集

	int          startVertex;         //起始顶点
	int          startIndex;          //
	int          primitiveCount;      //顶点个数

public:
	CIFCInstance();
	CIFCInstance(int InstanceID,CString GID,int OId,CString InstanceName);
	~CIFCInstance();
	BOOL Initialize(int ProjectID,int ModelType);
	BOOL GetSelected();                //返回是否可见
	void SetSelected(BOOL bSelecting); //设置可见性
	int  GetID();                      //返回实例ID
	CString GetGlobalID();             //返回全局唯一ID
	int GetOID();                      //返回物理文件中代表实例的行号
	int GetStartVertex();              //
	int GetStartIndex();               //
	int GetPrimitiveCount();           //
	CString GetName();                 //返回实例名称
};


/************************************************************************/
/*    IFC属性类（单个属性)                                              */
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
