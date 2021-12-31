#include "MainHeader.h"
#include "inGame.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "ParticleSystem.h"

void InGame::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");

	//LoadMesh("Dome", SCALING);
	//LoadMesh("MOdNA_Idle", SCALING);
	//LoadMesh("BossA", SCALING);
	//LoadMesh("BossB", SCALING);
	//LoadMesh("TerrainDecimate", SCALING);

	//LoadTerrainMesh("Terrain", "Terrain_RGB.png", SCALING);

	LoadTexture("Dump.png");
	//LoadTexture("ground01_Albedo.png");
	//LoadTexture("ground02_Albedo.png");
	//LoadTexture("ground01_Normal.png");
	//LoadTexture("ground02_Normal.png");

	LoadTexture("Particle_blast.png");
	LoadTexture("Particle_blast2.png");
	LoadTexture("Particle_embers4.png");
	LoadTexture("Particle_hotCloudColor.png");
	
	LoadTexture("Particle_hotCloud.png");
	LoadTexture("Particle_blast4.png");
	LoadTexture("Particle_glowSphere.png");
	LoadTexture("Particle_glowSphere2.png");
	LoadTexture("Particle_explosion.png");
	LoadTexture("Particle_ball88.png");
	LoadTexture("Particle_stand88.png");
	LoadTexture("Particle_glowBubbles.png");

	LoadTexture("Particle_Slash_1.png");
	LoadTexture("Particle_Slash_2.png");
	LoadTexture("Particle_Smoke_1.png");

	LoadTexture("Particle_Smoke_7.png");

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,5,-25 };

	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///터레인
	//testobj = InstanceTerrain("Terrain");
	//Terrain* terrain = testobj->GetComponent<Terrain>();
	//terrain->SetMeshName("Terrain");
	//terrain->SetLayerName("ground01_Albedo", "ground01_Normal");
	//terrain->SetLayerName("ground02_Albedo", "ground02_Normal");
	//terrain->SetColliderName("TerrainDecimate");
	//terrain->SetTextureTiling(1.0f / 31.0f);
	//testobj->GetTransform()->Rotation = { 0, 0, 0 };

	//testobj = Instance("BossA");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("BossA");
	//testobj->GetComponent<Transform>()->Position = { 10.f, 0.f, 0.f };
	//
	//testobj = Instance("BossB");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("BossB");
	//testobj->GetComponent<Transform>()->Position = { -10.f, 0.f, 0.f };

	/// 바닥
	//testobj = Instance("Field");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Dome");
	//testobj->GetComponent<Transform>()->Rotation = { 0.f, 0.f, 0.f };
	//testobj->GetComponent<Transform>()->Scale = { 1.0f, 1.0f, 1.0f };


	///캐릭터
	//testobj = Instance("MOdNA_Idle");
	//AC = testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>()->SetMeshName("MOdNA");
	//testobj->GetComponent<MeshFilter>()->SetAnimationName("MOdNA");
	//testobj->GetTransform()->Position = { 0,-10,0 };
	//AC->Choice("Idle");
	
	//testobj->AddComponent<Player>();



	///캐릭터
	//testobj = Instance("CHARACTER");
	//testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>();
	//testobj->GetComponent<MeshFilter>()->SetMeshName("CHARACTER");
	//testobj->GetComponent<MeshFilter>()->SetAnimationName("CHARACTER");
	//testobj->GetComponent<AnimationController>()->Choice("idle");
	//
	//testobj->GetTransform()->Scale = { 0.5f, 0.5f, 0.5f };
	//testobj->GetTransform()->Rotation = { -90.0f, 0.0f, 0.0f };
	//testobj->GetTransform()->Position = { 100.0f, 1.0f, 0.0f };

	//ParticleObj = Instance();
	//
	//testobj = InstanceParticle("Particle12");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_blast4");
	//particles->SetStartLifeTime(3.0f, 4.0f);
	//particles->SetStartSize(5.0f, 6.125f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 111, 0, 8), Vector4(255, 69, 0, 8));
	//particles->SetMaxParticles(40);
	//particles->SetRateOverTime(8.0f);
	//particles->SetShapeRadius(1.0f);      
	//particles->SetStartForce(Vector3(0, 10, 0));
	//particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);
	//
	//testobj = InstanceParticle("Particle9");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_glowSphere2");
	//particles->SetStartLifeTime(1.0f, 4.0f);
	//particles->SetStartSize(0.15f, 0.5f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 90, 0, 126), Vector4(255, 126, 0, 126));
	//particles->SetMaxParticles(100);
	//particles->SetRateOverTime(20.0f);
	//particles->SetShapeRadius(2.6125f);
	//particles->SetStartForce(Vector3(0, -25, 0));
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 255), Vector4(255, 255, 255, 0), PARTICLE_LIFETIME_OPTION::UP);
	////particles->SetLifeTimeSize(0.6f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	//particles->SetLifeTimeRotation(-45.0f, 45.0f);
	//particles->SetTextureTiling(1, 1);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);
	//
	//testobj = InstanceParticle("Particle8");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_hotCloudColor");
	//particles->SetStartLifeTime(0.8f, 1.25f);
	//particles->SetStartSize(5.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 111, 0, 83), Vector4(255, 174, 128, 80));
	//particles->SetMaxParticles(16);
	//particles->SetRateOverTime(8.0f);
	//particles->SetShapeRadius(0.375f);
	//particles->SetStartForce(Vector3(0, 10, 0));
	//particles->SetLifeTimeSize(0.75f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(-12.0f, 12.0f);
	//particles->SetTextureTiling(8, 8);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);
	//
	//testobj = InstanceParticle("Particle4");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_hotCloud");
	//particles->SetStartLifeTime(1.5f, 1.8f);
	//particles->SetStartSize(4.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 174, 73, 28), Vector4(255, 111, 53, 255));
	//particles->SetMaxParticles(60);
	//particles->SetRateOverTime(25.0f);
	//particles->SetShapeRadius(0.1875f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	////particles->SetLifeTimeSize(0.1f, 1.0f, PARTICLE_LIFETIME_OPTION::DOWN);
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);
	//
	//testobj = InstanceParticle("Particle5");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_hotCloudColor");
	//particles->SetStartLifeTime(1.65f, 2.2f);
	//particles->SetStartSize(5.0f, 7.0f);
	//particles->SetStartRotation(-360, 360);
	//particles->SetStartColor(Vector4(255, 174, 73, 28), Vector4(255, 111, 53, 255));
	//particles->SetMaxParticles(40);
	//particles->SetRateOverTime(15.0f);
	//particles->SetShapeRadius(0.1875f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	////particles->SetLifeTimeSize(0.1f, 1.0f, PARTICLE_LIFETIME_OPTION::DOWN);
	//particles->SetLifeTimeRotation(-15.0f, 15.0f);
	//particles->SetTextureTiling(8, 8);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);
	//
	//testobj = InstanceParticle("Particle11");
	//ParticleObj->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetDiffuseName("Particle_glowSphere");
	//particles->SetStartLifeTime(1.0f, 2.5f);
	//particles->SetStartSize(0.075f, 0.5f);
	//particles->SetStartRotation(-8, 8);
	//particles->SetStartColor(Vector4(255, 200, 0, 28), Vector4(255, 150, 0, 64));
	//particles->SetMaxParticles(150);
	//particles->SetRateOverTime(50.0f);
	//particles->SetShapeRadius(3.0f);
	//particles->SetStartForce(Vector3(0, 5, 0));
	//particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	//particles->SetLifeTimeColor(Vector4(255, 255, 255, 255), Vector4(255, 255, 255, 0), PARTICLE_LIFETIME_OPTION::UP);
	//particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(-32.0f, 32.0f);
	//particles->SetTextureTiling(1, 1);
	////particles->SetPlay(10.0f, true);
	//ParticleList.push_back(particles);

	//Particle_Slash_2
	//Particle_Smoke_1
	
	//testobj = InstanceParticle("Particle_2");
	//ParticleObj_1->ChoiceChild(testobj);
	//
	//particles = testobj->GetComponent<ParticleSystem>();
	//particles->SetMeshName("Quad");
	//particles->SetRenderType(PARTICLE_RENDER_OPTION::BILLBOARD);
	//particles->SetDiffuseName("Particle_Smoke_1");
	//particles->SetStartLifeTime(1.0f);
	//particles->SetStartSize(4.0f, 5.0f);
	//particles->SetStartRotation(-180, 180);
	//particles->SetStartColor(Vector4(88, 118, 255, 255));
	//particles->SetMaxParticles(200);
	//particles->SetRateOverTime(100.0f);
	//particles->SetShapeRadius(1.0f);
	//particles->SetStartForce(Vector3(0, 10, 0));
	//particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	//particles->SetLifeTimeColor(Vector4(0, 0, 0, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	////particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeRotation(0.0f);
	//particles->SetTextureTiling(10, 10);
	//particles->SetPlay(10.0f, true);
	//ParticleList_1.push_back(particles);

	ParticleObj_2 = Instance();
	ParticleObj_2->transform->Position.x -= 20.0f;

	testobj = InstanceParticle("Particle_2");
	testobj->transform->Rotation.y = -90.0f;
	ParticleObj_2->ChoiceChild(testobj);

	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::MESH);
	particles->SetDiffuseName("Particle_Slash_1");
	particles->SetDelayTime(1.0f);
	particles->SetStartLifeTime(0.25f);
	particles->SetStartSize(10.0f);
	particles->SetStartRotation(-90);
	particles->SetStartColor(Vector4(192, 152, 48, 255));
	particles->SetMaxParticles(2);
	particles->SetRateOverTime(1.0f);
	particles->SetShapeRadius(0.01f);
	particles->SetStartForce(Vector3(0, 0, 0));
	particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles->SetLifeTimeColor(Vector4(0, 0, 0, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeRotation(0.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(0.25f);
	ParticleList_2.push_back(particles);


	ParticleObj_1 = Instance();

	testobj = InstanceParticle("Particle_1");
	ParticleObj_1->ChoiceChild(testobj);

	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
	particles->SetDiffuseName("Particle_Slash_2");
	particles->SetStartLifeTime(2.0f);
	particles->SetStartSize(10.0f);
	particles->SetStartRotation(-180, 180);
	particles->SetStartColor(Vector4(88, 118, 255, 255));
	particles->SetMaxParticles(20);
	particles->SetRateOverTime(10.0f);
	particles->SetShapeRadius(0.01f);
	particles->SetStartForce(Vector3(0, 0, 0));
	particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles->SetLifeTimeColor(Vector4(0, 0, 0, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeRotation(360.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f, true);
	ParticleList_1.push_back(particles);

	ParticleObj_3 = Instance();
	ParticleObj_3->transform->Position.x += 20.0f;

	testobj = InstanceParticle("Particle_3");
	testobj->transform->Rotation.y = -45.0f;
	ParticleObj_3->ChoiceChild(testobj);

	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::MESH);
	particles->SetDiffuseName("Particle_Slash_2");
	particles->SetStartLifeTime(0.5f);
	particles->SetStartSize(10.0f);
	particles->SetStartRotation(-90);
	particles->SetStartColor(Vector4(192, 30, 48, 255));
	particles->SetMaxParticles(2);
	particles->SetRateOverTime(0.5f);
	particles->SetShapeRadius(0.01f);
	particles->SetStartForce(Vector3(0, 0, 0));
	particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles->SetLifeTimeColor(Vector4(255, 0, 0, 0), Vector4(100, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles->SetLifeTimeSize(0.5f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeRotation(0.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(10.0f);
	ParticleList_3.push_back(particles);

	//testobj = Instance("Field1");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->GetComponent<Transform>()->Rotation = { -90.0f, 90.0f, 0.0f };
	//testobj->GetComponent<Transform>()->Scale = { 0.05f, 0.05f, 0.05f };
	//testobj->GetComponent<Transform>()->Position = { 0.0f, 2.5f, 2.5f };

	//testobj = Instance("Table");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Table");
	//testobj->GetComponent<Transform>()->Scale = { 0.1f, 0.1f, 0.1f };
	//testobj->GetTransform()->Position = { -5.0f, 0.0f, -5.0f };
}

void InGame::Start()
{
}

void InGame::Update()
{
	if (GetKey(VK_LEFT) == true)
	{
		ParticleObj->GetTransform()->Position.x -= 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_RIGHT) == true)
	{
		ParticleObj->GetTransform()->Position.x += 10.0f * GetDeltaTime();
	}

	if (GetKey(VK_UP) == true)
	{
		ParticleObj->GetTransform()->Position.z += 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_DOWN) == true)
	{
		ParticleObj->GetTransform()->Position.z -= 10.0f * GetDeltaTime();
	}

	//if (GetKeyUp(VK_RETURN))
	//{
	//	for (ParticleSystem* p : ParticleList)
	//	{
	//		p->SetPlay(10.f, true);
	//	}
	//}

	if (GetKeyUp(VK_F1))
	{
		for (ParticleSystem* p : ParticleList_1)
		{
			p->Play();
		}
	}

	if (GetKeyUp(VK_F2))
	{
		for (ParticleSystem* p : ParticleList_2)
		{
			p->Play();
		}
	}

	if (GetKeyUp(VK_F3))
	{
		for (ParticleSystem* p : ParticleList_3)
		{
			p->Play();
		}
	}


	if (GetKeyUp(VK_F4))
	{
		for (ParticleSystem* p : ParticleList_1)
		{
			p->Play(true);
		}
		for (ParticleSystem* p : ParticleList_2)
		{
			p->Play(true);
		}
		for (ParticleSystem* p : ParticleList_3)
		{
			p->Play(true);
		}
	}
}

void InGame::End()
{
}
