// HLSL ��� (���� ���� ���̴� ������)
// ���� ���̴�. 
// �������̴��� �������ۿ� �ִ� ��� ������ �ѹ��� �۾��Ѵ�.
// ������ġ�� �ݵ�� float4�� SV_POSITION �������Ϳ� ��ȯ�ؾ� �Ѵ�. (������ǥ��? �ϴ� ������ ��?�絵 ��)
// POSITION -> �ø�ƽ : �ṉ̀���, �̸��� �ڴ�� ���ص� �Ǳ�� �� ����


// Vertex Shader,�ٿ��� VS       �� ��� ����            
float4 VS(float3 p : POSITION) : SV_POSITION // <- �ý��� ��� ������, �ݵ�� ������ ���(SV_POSITION)�� ������ġ�� �Ѱ������.
{
	//float4(px, py, pz, 1.0f) == float4(p, 1.0f) �̷��� �������̶�� �Ѵ�
	return float4(p, 1.0f);
}

//float4 VS(float3 p : POSITION, float4 c : COLOR) : SV_POSITION       
//{
//	return float4(p, 1.0f);
//}