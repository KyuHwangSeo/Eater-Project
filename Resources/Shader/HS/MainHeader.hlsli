//카메라 관련 버퍼
cbuffer CameraBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
};

//오브젝트 관련 버퍼
cbuffer ObjectBuffer : register(b1)
{
    float4x4    world;
};

//스키닝 관련 버퍼
cbuffer SkinningBuffer : register(b2)
{
    float4x4 Skinning_world;
    float4x4 BoneOffset[96];
};

Texture2D MainTexture   : register(t0); //메인
Texture2D NomalTexture  : register(t1); //노말

SamplerState SampleType : register(s0)
{
    Filter = MIN_MAG_MIP_POINT; //텍스쳐를 샘플링 할때 사용할 필터 방식

    AddressU = BORDER; //0~1범위를 벗어나는 u텍스쳐 좌표를해결하는방법
    AddressV = BORDER; //0~1범위를 벗어나는 v텍스쳐 좌표를해결하는방법
    AddressW = BORDER; //0~1범위를 벗어나는 w텍스쳐 좌표를해결하는방법


    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f); //D3D1_TEX일경우 사용할 테두리 색주소 0~1사이
    ComparisonFunc = LESS_EQUAL; //샘플링된 데이트를 기존 새믈링된 데이터와 비교하는 함수
    //LESS_EQUAL = 소스 데이터가 대상 데이터보다 작거나 같으면 통과
};