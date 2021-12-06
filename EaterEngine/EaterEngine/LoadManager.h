#pragma once

/// <summary>
/// 매쉬,텍스쳐,프리팹,등등 그리기위해서 필요한것들을 로드
/// 
/// 
/// 테스트용으로 동혁이꺼 랜더링 엔진이 붙어있음
/// </summary>



#include <string>
#include <map>
#include "ParserData.h"
#include "SimpleMath.h"

namespace ParserData
{
	struct Model;
	class Mesh;
	class OneAnimation;
}

class ModelAnimationData;
class LoadMeshData;
class ModelData;
class ModelParser;
class FBXParser;
class FBXModel;
class TextureBuffer;
class GraphicEngineManager;

class LoadManager
{
public:
	LoadManager();
	~LoadManager();

	//초기화 및 경로 설정
	void Initialize(GraphicEngineManager* Graphic);
public:
	///GET
	//매쉬 가져오기
	static ModelData* GetMesh(std::string Name);
	//텍스쳐 가져오기
	static TextureBuffer* GetTexture(std::string Name);
	//애니메이션 가져오기
	static ModelAnimationData* GetAnimation(std::string Name);
	
	///Load
	//모델 로드(스크린 이름,모델의 이름,스케일 여부,애니메이션 여부)
	void LoadMesh(std::string Name, bool Scale = true,bool LoadAnime = false);
	void LoadTexture(std::string Name);
	//프리펩 로드
	void LoadPrefap(std::string Name);

	///경로
	//매쉬 경로 설정
	void LoadMeshPath(std::string mPath);
	//텍스쳐 경로
	void LoadTexturePath(std::string mPath);

	///Delete
	//리스트에서 매쉬정보를 삭제(매쉬가 너무많아서 메모리가 부족할때)
	void DeleteMesh(std::string mMeshName);
	//모든 매쉬정보를 삭제
	void DeleteMeshAll();
private:
	//계층 구조를 타지않고 탑오브젝트들만 읽어온다
	LoadMeshData* CreateMeshObjeect(ParserData::Mesh* mesh);
	LoadMeshData* CreateBoneObjeect(ModelData* SaveData);

	//애니메이션을 로드함
	void LoadAnimation(ModelData* SaveMesh,ParserData::Model* MeshData,std::string Name);
	//로드한 애니메이션의키를 보간후 키를넣어줌
	void CreateAnimationKeyFrame(std::vector<ParserData::OneAnimation*>* Anime, int InputKeyCount);
	//파싱한데이터에서 값을 변경하지않은것들을 그대로 복사함
	void SetData(LoadMeshData* MeshData, ParserData::Mesh* LoadData);
private:
	///모델이 들어있는 경로
	std::string MeshPath;
	///텍스쳐가 들어있는 경로
	std::string TexturePath;
	///리스트
	static std::map<std::string, ModelData*>			ModelList;
	static std::map<std::string, TextureBuffer*>		TextureList;
	static std::map<std::string, ModelAnimationData*>	AnimationList;
	//규황이 파서
	ModelParser* EaterParser;
	//누군가의 그래픽 엔진
	GraphicEngineManager* GEngine;
};
