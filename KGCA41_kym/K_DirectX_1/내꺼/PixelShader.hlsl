// 픽셀마다 호출된다

// 해달 픽쉘의 컬러를 반환한다. 어디에? SV_TARGET 레지스터에
float4 PS(float4 p : SV_POSITION) : SV_TARGET
{
	return float4(1,1,0,1);
}