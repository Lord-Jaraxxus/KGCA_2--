#include "K_FBXLoader.h"

bool K_FBXLoader::Init()
{
    m_pFbxManager = FbxManager::Create();
    m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");

	// 단위
	FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
	// 기저(행렬)
	FbxAxisSystem::MayaZUp.ConvertScene(m_pFbxScene);

    return true;
}

bool K_FBXLoader::Frame()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Frame();
	}
	return true;
}

bool K_FBXLoader::Render()
{
	for (auto obj : m_pDrawObjListS)
	{
		obj->Render();
	}
	return true;
}

bool K_FBXLoader::Release()
{
	for (auto obj : m_pDrawObjList)
	{
		obj->Release();
	}

	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();
	return true;
}


bool K_FBXLoader::Load(C_STR filename)
{
	m_pFbxImporter->Initialize(filename.c_str());
	m_pFbxImporter->Import(m_pFbxScene);
	m_pRootNode = m_pFbxScene->GetRootNode();
	InitAnimation();
	//PreProcess(m_pRootNode);
	PreProcessS(m_pRootNode);

	// animation
	FbxTime time;
	for (FbxLongLong t = m_AnimeScene.iStartFrame; t <= m_AnimeScene.iEndFrame; t++)
	{
		time.SetFrame(t, m_AnimeScene.TimeMode);
		LoadAnimationS(t, time);
	}

	// 이런게 일단 필요해보이긴 하는데 일단 가야.. 가야하나? 
	for (auto tObj : m_pObjectListS)
	{
		if (tObj->m_pFbxParentNode != nullptr)
		{
			auto data = m_pObjectMap.find(tObj->m_pFbxParentNode);
			tObj->SetParent(data->second);
		}
		FbxMesh* pFbxMesh = tObj->m_pFbxNode->GetMesh();
		if (pFbxMesh)
		{
			ParseMeshS(pFbxMesh, tObj);
		}
	} 

	return true;
}

// fbx 트리를 쭉 돌면서 노드를 FbxObject에 담아옴
void K_FBXLoader::PreProcess(FbxNode* pFbxNode)
{
	if (pFbxNode == nullptr) return;

	K_FbxObject* pObject = new K_FbxObject;
	pObject->m_pFbxNode = pFbxNode;
	pObject->m_pFbxParentNode = pFbxNode->GetParent();
	if (pObject->m_pFbxParentNode != nullptr) 
	{
		for (auto tObj : m_pObjectList)
		{
			if (tObj->m_pFbxNode == pObject->m_pFbxParentNode) pObject->SetParent(tObj);
		}
	}
	LoadAnimation(pObject);
	m_pObjectList.push_back(pObject);

	FbxMesh* pFbxMesh = pFbxNode->GetMesh();
	if (pFbxMesh)
	{
		m_pFbxMeshList.push_back(pFbxMesh);
		ParseMesh(pFbxMesh, pObject);
	}

	int iNumChild = pFbxNode->GetChildCount();
	for (int iChild = 0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChild = pFbxNode->GetChild(iChild);

		FbxNodeAttribute::EType type = pChild->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eSkeleton ||
			type == FbxNodeAttribute::eNull)
		{
			PreProcess(pChild);
		}
	}
}

// fbx 트리를 쭉 돌면서 노드를 FbxObject에 담아옴, Skinning 버전
void K_FBXLoader::PreProcessS(FbxNode* pFbxNode)
{
	if (pFbxNode == nullptr) return;

	K_FbxObjectSkinning* pObject = new K_FbxObjectSkinning;
	pObject->m_pFbxNode = pFbxNode;
	pObject->m_pFbxParentNode = pFbxNode->GetParent();
	pObject->m_iObjectBone = m_pObjectListS.size();

	if (pObject->m_pFbxParentNode != nullptr)
	{
		for (auto tObj : m_pObjectListS)
		{
			if (tObj->m_pFbxNode == pObject->m_pFbxParentNode) pObject->SetParent(tObj);
		}
	}

	m_pObjectListS.push_back(pObject);
	m_pObjectMap.insert(std::make_pair(pFbxNode, pObject));
	m_pObjectIDMap.insert(std::make_pair(pFbxNode, pObject->m_iObjectBone));

	int iNumChild = pFbxNode->GetChildCount();
	for (int iChild = 0; iChild < iNumChild; iChild++)
	{
		FbxNode* pChild = pFbxNode->GetChild(iChild);

		FbxNodeAttribute::EType type = pChild->GetNodeAttribute()->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eSkeleton ||
			type == FbxNodeAttribute::eNull)
		{
			PreProcessS(pChild);
		}
	}
}


// 메쉬 분석, 우리한테 필요한 데이터(버텍스정점, 텍스처이름, 텍스처 UV등등) 갖고옴
void K_FBXLoader::ParseMesh(FbxMesh* pFbxMesh, K_FbxObject* pObject)
{
	FbxNode* pNode = pFbxMesh->GetNode();

	FbxAMatrix geom; // 기하(로칼)행렬(초기 정점 위치를 변환할 때 사용한다.)
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalLocalMatrix = geom;
	normalLocalMatrix = normalLocalMatrix.Inverse();
	normalLocalMatrix = normalLocalMatrix.Transpose();

	//// 월드행렬
	//FbxVector4 Translation;
	//if (pNode->LclTranslation.IsValid())
	//	Translation = pNode->LclTranslation.Get();

	//FbxVector4 Rotation;
	//if (pNode->LclRotation.IsValid())
	//	Rotation = pNode->LclRotation.Get();

	//FbxVector4 Scale;
	//if (pNode->LclScaling.IsValid())
	//	Scale = pNode->LclScaling.Get();

	//FbxAMatrix matWorldTransform(Translation, Rotation, Scale);
	//FbxAMatrix normalWorldMatrix = matWorldTransform;
	//normalWorldMatrix = normalWorldMatrix.Inverse();
	//normalWorldMatrix = normalWorldMatrix.Transpose();

	// Layer 개념
	FbxLayerElementUV* VertexUVSet = nullptr;
	FbxLayerElementVertexColor* VertexColorSet = nullptr;
	FbxLayerElementNormal* VertexNormalSet = nullptr;
	FbxLayerElementMaterial* MaterialSet = nullptr;
	FbxLayer* pFbxLayer = pFbxMesh->GetLayer(0);

	if (pFbxLayer->GetUVs() != nullptr) VertexUVSet = pFbxLayer->GetUVs();
	if (pFbxLayer->GetVertexColors() != nullptr) VertexColorSet = pFbxLayer->GetVertexColors();
	if (pFbxLayer->GetNormals() != nullptr) VertexNormalSet = pFbxLayer->GetNormals();
	if (pFbxLayer->GetMaterials() != nullptr) MaterialSet = pFbxLayer->GetMaterials();


	std::string szFileName;
	int iNumMtrl = pNode->GetMaterialCount();
	std::vector<C_STR> texList;
	texList.resize(iNumMtrl);

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		// 24 이상의 정보가 있다.
		FbxSurfaceMaterial* pSurface = pNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (property.IsValid())
			{
				const FbxFileTexture* tex = property.GetSrcObject<FbxFileTexture>(0);
				if (tex != nullptr) 
				{
					szFileName = tex->GetFileName();
					texList[iMtrl] = szFileName;
				}
			}
		}
	}

	pObject->m_vVertexDataListList.resize(iNumMtrl);
	pObject->m_vTextureNameList.resize(iNumMtrl);
	for (int iTex = 0; iTex < iNumMtrl; iTex++)
	{
		pObject->m_vTextureNameList[iTex] = I_Tex.GetSplitName(texList[iTex]); 
	}
	if (iNumMtrl < 1) // 서브 메테리얼, 즉 텍스처가 하나도 없을 경우
	{
		pObject->m_vVertexDataListList.resize(1);
		pObject->m_vTextureNameList.resize(1);
		pObject->m_vTextureNameList[0] = L"Default.jpg"; // 일단 임시로 뭐라도 쓰라고 갖다줌
	}


	int iNumPolyCount = pFbxMesh->GetPolygonCount();
	// 3 정점 -> 1폴리곤( 삼각형)
	// 4 정점 -> 1폴리곤( 쿼드 )
	int iNumFace = 0;
	int iBasePolyIndex = 0;
	int iSubMtrl = 0;

	// 제어점
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;

		// 얘는 뭐 현재 몇번째 sm인지 알려주는 거야 아니면 총 sm이 몇개인지 알려주는거야? 
		if(MaterialSet) iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet);

		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// 정점 인덱스
			int iCornerIndex[3];
			iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 2);
			iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 1);

			// 텍스처 UV 인덱스같은데,얘는 왜 따로 있는겨?
			int iUVIndex[3];
			iUVIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
			iUVIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
			iUVIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

			// 정점컬러인덱스
			int VertexColor[3] = { 0, iFace + 2, iFace + 1 };

			// 아무튼 for문 돌면서 필요한 정보들 수집
			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				// p, position (버텍스 정점)
				int vertexID = iCornerIndex[iIndex];
				FbxVector4 v2 = pVertexPositions[vertexID];
				PNCT_VERTEX tVertex;
				FbxVector4 v = geom.MultT(v2);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];

				// n, normal (정점 노말)
				if (VertexNormalSet)
				{
					FbxVector4 n = ReadNormal(
						pFbxMesh,
						VertexNormalSet,
						iCornerIndex[iIndex],
						iBasePolyIndex + VertexColor[iIndex]);
					n = normalLocalMatrix.MultT(n);
					tVertex.n.x = n.mData[0];
					tVertex.n.y = n.mData[2];
					tVertex.n.z = n.mData[1];
				}

				// c, color (정점 컬러)
				tVertex.c = k_Vector4D(1, 1, 1, 1);
				if (VertexColorSet)
				{
					FbxColor c = ReadColor(
						pFbxMesh,
						VertexColorSet,
						iCornerIndex[iIndex],
						iBasePolyIndex + VertexColor[iIndex]);
					tVertex.c.x = c.mRed;
					tVertex.c.y = c.mGreen;
					tVertex.c.z = c.mBlue;
					tVertex.c.w = 1.0f;
				}

				//  t, texture (텍스처 UV좌표)
				if (VertexUVSet) 
				{
					FbxVector2 t = ReadTextureCoord(pFbxMesh, VertexUVSet, iCornerIndex[iIndex], iUVIndex[iIndex]);
					tVertex.t.x = t.mData[0];
					tVertex.t.y = 1.0f - t.mData[1];
				}
				pObject->m_vVertexDataListList[iSubMtrl].push_back(tVertex); // 이거 그냥 이렇게 통일해도?
			}
		}
		iBasePolyIndex += iPolySize;
	}

	m_pDrawObjList.push_back(pObject);
}

// 메쉬 분석, Skinning 버전
void K_FBXLoader::ParseMeshS(FbxMesh* pFbxMesh, K_FbxObjectSkinning* pObject)
{
	pObject->m_bSkinned = ParseMeshSkinning(pFbxMesh, pObject); // 스키닝이 있으면 true 업스면 false

	FbxNode* pNode = pFbxMesh->GetNode();

	FbxAMatrix geom; // 기하(로칼)행렬(초기 정점 위치를 변환할 때 사용한다.)
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalLocalMatrix = geom;
	normalLocalMatrix = normalLocalMatrix.Inverse();
	normalLocalMatrix = normalLocalMatrix.Transpose();

	//// 월드행렬
	//FbxVector4 Translation;
	//if (pNode->LclTranslation.IsValid())
	//	Translation = pNode->LclTranslation.Get();

	//FbxVector4 Rotation;
	//if (pNode->LclRotation.IsValid())
	//	Rotation = pNode->LclRotation.Get();

	//FbxVector4 Scale;
	//if (pNode->LclScaling.IsValid())
	//	Scale = pNode->LclScaling.Get();

	//FbxAMatrix matWorldTransform(Translation, Rotation, Scale);
	//FbxAMatrix normalWorldMatrix = matWorldTransform;
	//normalWorldMatrix = normalWorldMatrix.Inverse();
	//normalWorldMatrix = normalWorldMatrix.Transpose();

	// Layer 개념
	FbxLayerElementUV* VertexUVSet = nullptr;
	FbxLayerElementVertexColor* VertexColorSet = nullptr;
	FbxLayerElementNormal* VertexNormalSet = nullptr;
	FbxLayerElementMaterial* MaterialSet = nullptr;
	FbxLayer* pFbxLayer = pFbxMesh->GetLayer(0);

	if (pFbxLayer->GetUVs() != nullptr) VertexUVSet = pFbxLayer->GetUVs();
	if (pFbxLayer->GetVertexColors() != nullptr) VertexColorSet = pFbxLayer->GetVertexColors();
	if (pFbxLayer->GetNormals() != nullptr) VertexNormalSet = pFbxLayer->GetNormals();
	if (pFbxLayer->GetMaterials() != nullptr) MaterialSet = pFbxLayer->GetMaterials();


	std::string szFileName;
	int iNumMtrl = pNode->GetMaterialCount();
	std::vector<C_STR> texList;
	texList.resize(iNumMtrl);

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		// 24 이상의 정보가 있다.
		FbxSurfaceMaterial* pSurface = pNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			auto property = pSurface->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (property.IsValid())
			{
				const FbxFileTexture* tex = property.GetSrcObject<FbxFileTexture>(0);
				if (tex != nullptr)
				{
					szFileName = tex->GetFileName();
					texList[iMtrl] = szFileName;
				}
			}
		}
	}

	pObject->m_vVertexDataListList.resize(iNumMtrl);
	pObject->m_vVertexDataListList_IW.resize(iNumMtrl);
	pObject->m_vTextureNameList.resize(iNumMtrl);
	for (int iTex = 0; iTex < iNumMtrl; iTex++)
	{
		pObject->m_vTextureNameList[iTex] = I_Tex.GetSplitName(texList[iTex]);
	}
	if (iNumMtrl < 1) // 서브 메테리얼, 즉 텍스처가 하나도 없을 경우
	{
		pObject->m_vVertexDataListList.resize(1);
		pObject->m_vTextureNameList.resize(1);
		pObject->m_vVertexDataListList_IW.resize(1);
		pObject->m_vTextureNameList[0] = L"Default.jpg"; // 일단 임시로 뭐라도 쓰라고 갖다줌
	}


	int iNumPolyCount = pFbxMesh->GetPolygonCount();
	// 3 정점 -> 1폴리곤( 삼각형)
	// 4 정점 -> 1폴리곤( 쿼드 )
	int iNumFace = 0;
	int iBasePolyIndex = 0;
	int iSubMtrl = 0;

	// 제어점
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;

		// 얘는 뭐 현재 몇번째 sm인지 알려주는 거야 아니면 총 sm이 몇개인지 알려주는거야? 
		if (MaterialSet) iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet);

		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// 정점 인덱스
			int iCornerIndex[3];
			iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 2);
			iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 1);

			// 텍스처 UV 인덱스같은데,얘는 왜 따로 있는겨?
			int iUVIndex[3];
			iUVIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
			iUVIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
			iUVIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

			// 정점컬러인덱스
			int VertexColor[3] = { 0, iFace + 2, iFace + 1 };

			// 아무튼 for문 돌면서 필요한 정보들 수집
			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				// p, position (버텍스 정점)
				int vertexID = iCornerIndex[iIndex];
				FbxVector4 v2 = pVertexPositions[vertexID];
				PNCT_VERTEX tVertex;
				IW_VERTEX	IWVertex;
				FbxVector4 v = geom.MultT(v2);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];

				// n, normal (정점 노말)
				if (VertexNormalSet)
				{
					FbxVector4 n = ReadNormal(
						pFbxMesh,
						VertexNormalSet,
						iCornerIndex[iIndex],
						iBasePolyIndex + VertexColor[iIndex]);
					n = normalLocalMatrix.MultT(n);
					tVertex.n.x = n.mData[0];
					tVertex.n.y = n.mData[2];
					tVertex.n.z = n.mData[1];
				}

				// c, color (정점 컬러)
				tVertex.c = k_Vector4D(1, 1, 1, 1);
				if (VertexColorSet)
				{
					FbxColor c = ReadColor(
						pFbxMesh,
						VertexColorSet,
						iCornerIndex[iIndex],
						iBasePolyIndex + VertexColor[iIndex]);
					tVertex.c.x = c.mRed;
					tVertex.c.y = c.mGreen;
					tVertex.c.z = c.mBlue;
					tVertex.c.w = 1.0f;
				}

				//  t, texture (텍스처 UV좌표)
				if (VertexUVSet)
				{
					FbxVector2 t = ReadTextureCoord(pFbxMesh, VertexUVSet, iCornerIndex[iIndex], iUVIndex[iIndex]);
					tVertex.t.x = t.mData[0];
					tVertex.t.y = 1.0f - t.mData[1];
				}

				// Index랑 Weight, 음.. 난 IDMap이 없는데! 업스면 만드러 임마
				if (pObject->m_bSkinned == false)
				{
					IWVertex.i.x = m_pObjectIDMap.find(pNode)->second;
					IWVertex.i.y = 0;
					IWVertex.i.z = 0;
					IWVertex.i.w = 0;
					IWVertex.w.x = 1.0f;
					IWVertex.w.y = 0.0f;
					IWVertex.w.z = 0.0f;
					IWVertex.w.w = 0.0f;
				}
				else
				{
					IndexWeight* pWeight = &pObject->m_IndexWeightList[vertexID];
					IWVertex.i.x = pWeight->Index[0];
					IWVertex.i.y = pWeight->Index[1];
					IWVertex.i.z = pWeight->Index[2];
					IWVertex.i.w = pWeight->Index[3];
					IWVertex.w.x = pWeight->weight[0];
					IWVertex.w.y = pWeight->weight[1];
					IWVertex.w.z = pWeight->weight[2];
					IWVertex.w.w = pWeight->weight[3];
				}

				pObject->m_vVertexDataListList[iSubMtrl].push_back(tVertex); // 이거 그냥 이렇게 통일해도?
				pObject->m_vVertexDataListList_IW[iSubMtrl].push_back(IWVertex);
			}
		}
		iBasePolyIndex += iPolySize;
	}

	m_pDrawObjListS.push_back(pObject);
}


FbxVector2 K_FBXLoader::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 t;

	/*enum EMappingMode
	{
		eNone,				// 매핑이 결정되지 않았다.
		eByControlPoint,	// 제어점 및 정점에 1개의 매핑이 되어 있다.
		eByPolygonVertex,	// 각 정점이 속한 폴리곤의 매핑에 좌표가 있다.
		eByPolygon,         // 전체 폴리곤에 매핑 좌표가 있다.
		eByEdge,			// 에지에 1개의 매핑 좌표가 있다.
		eAllSame			// 전체 폴리곤에 1개의 매핑 좌표가 있다.
	};*/

	//enum EReferenceMode
	//{
	//	eDirect,   // n번째 매핑정보가 mDirectArray의 n번째 위치에 있다.
	//	eIndex,    // fbx 5.0 이하 버전에서 사용되었었다. 이상 버전에서는 eIndexToDirect로 대체되었다.
	//	eIndexToDirect
	//};


	// 텍스처 매핑 방식이 뭐냐?
	FbxLayerElement::EMappingMode mode = pUVSet->GetMappingMode();

	switch (mode)
	{

	case FbxLayerElementUV::eByControlPoint:
	{
		switch (pUVSet->GetReferenceMode())
		{

		case FbxLayerElementUV::eDirect:
		{
			t = pUVSet->GetDirectArray().GetAt(vertexIndex);
		}break;

		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = pUVSet->GetIndexArray().GetAt(vertexIndex);
			t = pUVSet->GetDirectArray().GetAt(index);
		}break;

		}break;
	} break;

	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (pUVSet->GetReferenceMode())
		{
		// 두개를 통으로 처리, 따로 놓으니까 몬가몬가 텍스처가 비스듬하게 붙었다고 카더라요
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			t = pUVSet->GetDirectArray().GetAt(uvIndex);
		}break;
		}break;
	}break;
	}

	return t;
}

FbxColor K_FBXLoader::ReadColor(FbxMesh* pFbxMesh,
	FbxLayerElementVertexColor* VertexColorSet,
	int vertexIndex,
	int colorIndex)
{
	FbxColor color(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = VertexColorSet->GetMappingMode();
	switch (mode)
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = VertexColorSet->GetDirectArray().GetAt(vertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexColorSet->GetIndexArray().GetAt(vertexIndex);
			color = VertexColorSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	} break;
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (VertexColorSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			color = VertexColorSet->GetDirectArray().GetAt(colorIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexColorSet->GetIndexArray().GetAt(colorIndex);
			color = VertexColorSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	}break;
	}
	return color;
}

FbxVector4 K_FBXLoader::ReadNormal(FbxMesh* pFbxMesh,
	FbxLayerElementNormal* VertexNormalSet,
	int vertexIndex,
	int normalIndex)
{
	FbxVector4 normal(1, 1, 1, 1);
	FbxLayerElement::EMappingMode mode = VertexNormalSet->GetMappingMode();
	switch (mode)
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = VertexNormalSet->GetDirectArray().GetAt(vertexIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexNormalSet->GetIndexArray().GetAt(vertexIndex);
			normal = VertexNormalSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	} break;
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (VertexNormalSet->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			normal = VertexNormalSet->GetDirectArray().GetAt(normalIndex);
		}break;
		case FbxLayerElementUV::eIndexToDirect:
		{
			int index = VertexNormalSet->GetIndexArray().GetAt(normalIndex);
			normal = VertexNormalSet->GetDirectArray().GetAt(index);
		}break;
		}break;
	}break;
	}
	return normal;
}

int K_FBXLoader::GetSubMaterialIndex(int iPoly, FbxLayerElementMaterial* pMaterialSetList)
{
	// 매핑방식
	//eNone,
	//eByControlPoint,  // 제어점
	//eByPolygonVertex, //  
	//eByPolygon, // 폴리곤마다 다를수 있다.
	//eAllSame - 전체표면에 1개의 매핑좌표가 있다.

	int iSubMtrl = 0;
	if (pMaterialSetList != nullptr)
	{
		switch (pMaterialSetList->GetMappingMode())
		{
		case FbxLayerElement::eByPolygon:
		{
			// 매핑 정보가 배열에 저장되는 방식
			switch (pMaterialSetList->GetReferenceMode())
			{
			case FbxLayerElement::eIndex:
			{
				iSubMtrl = iPoly;
			}break;
			case FbxLayerElement::eIndexToDirect:
			{
				iSubMtrl = pMaterialSetList->GetIndexArray().GetAt(iPoly);
			}break;
			}
		}
		default:
		{
			break;
		}
		}
	}
	return iSubMtrl;
}

void K_FBXLoader::InitAnimation()
{
	FbxAnimStack* stackAnim = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	FbxLongLong s = 0;
	FbxLongLong n = 0;
	FbxTime::EMode TimeMode;

	if (stackAnim) 
	{
		FbxString takeName = stackAnim->GetName();
		FbxTakeInfo* take = m_pFbxScene->GetTakeInfo(takeName);
		FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
		TimeMode = FbxTime::GetGlobalTimeMode();
		FbxTimeSpan localTimeSpan = take->mLocalTimeSpan;
		FbxTime start = localTimeSpan.GetStart();
		FbxTime end = localTimeSpan.GetStop();
		FbxTime Duration = localTimeSpan.GetDirection();
		s = start.GetFrameCount(TimeMode);
		n = end.GetFrameCount(TimeMode);
	}

	m_AnimeScene.iStartFrame = s;
	m_AnimeScene.iEndFrame = n;
	m_AnimeScene.fFrameSpeed = 30.0f;
	m_AnimeScene.fTickPerFrame = 160;
	m_AnimeScene.TimeMode = TimeMode;
}

void K_FBXLoader::LoadAnimation(K_FbxObject* pObject)
{
	FbxNode* pNode = pObject->m_pFbxNode;
	FbxTime time;

	for (FbxLongLong t = m_AnimeScene.iStartFrame; t <= m_AnimeScene.iEndFrame; t++)
	{
		time.SetFrame(t, m_AnimeScene.TimeMode);
		K_AnimeTrack track;
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);
		track.matAnime = ConvertMatrix(fbxMatrix);
		TBASIS_EX::TMatrix TM = ConvertMatrixKtoT(track.matAnime);
		TBASIS_EX::D3DXMatrixDecompose(&track.s, &track.r, &track.t, &TM); // 최종 월드?행렬을 srt로 쪼갬
		pObject->m_AnimeTracks.push_back(track);
	}
}

void K_FBXLoader::LoadAnimationS(FbxLongLong t, FbxTime time)
{
	for (auto tObj : m_pObjectListS)
	{
		FbxNode* pNode = tObj->m_pFbxNode;

		K_AnimeTrack track;
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);
		track.matAnime = ConvertMatrix(fbxMatrix);
		TBASIS_EX::TMatrix TM = ConvertMatrixKtoT(track.matAnime);
		TBASIS_EX::D3DXMatrixDecompose(&track.s, &track.r, &track.t, &TM); // 최종 월드?행렬을 srt로 쪼갬
		tObj->m_AnimeTracks.push_back(track);
	}
}


K_Matrix K_FBXLoader::ConvertMatrix(FbxMatrix fbxMatrix)
{
	K_Matrix orgin_mat;

	float* kArray = (float*)(&orgin_mat);
	double* fbxArray = (double*)(&fbxMatrix);
	for (int i = 0; i < 16; i++){ kArray[i] = fbxArray[i]; }


	K_Matrix result_mat;

	result_mat._11 = orgin_mat._11; result_mat._12 = orgin_mat._13; result_mat._13 = orgin_mat._12;
	result_mat._21 = orgin_mat._31; result_mat._22 = orgin_mat._33; result_mat._23 = orgin_mat._32;
	result_mat._31 = orgin_mat._21; result_mat._32 = orgin_mat._23; result_mat._33 = orgin_mat._22;
	result_mat._41 = orgin_mat._41; result_mat._42 = orgin_mat._43; result_mat._43 = orgin_mat._42;
	result_mat._14 = result_mat._24 = result_mat._34 = 0.0f;
	result_mat._44 = 1.0f;

	return result_mat;
}

HRESULT K_FBXLoader::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	for (int iBone = 0; iBone < 255; iBone++)
	{
		m_cbDataBone.matBone[iBone].Identity();
	}


	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VS_CONSTANT_BONE_BUFFER) * 1; // 바이트 용량
	// GPU 메모리에 할당
	bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_cbDataBone;
	hr = pDevice->CreateBuffer(
		&bd, // 버퍼 할당
		&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
		&m_pConstantBufferBone);

	return hr;
}

bool K_FBXLoader::UpdateFrame(ID3D11DeviceContext* pContext)
{
	m_fAnimeFrame = m_fAnimeFrame + g_fSecondPerFrame * m_fAnimeSpeed * m_AnimeScene.fFrameSpeed * m_fAnimeInverse;
	if (m_fAnimeFrame > m_AnimeScene.iEndFrame || m_fAnimeFrame < m_AnimeScene.iStartFrame)
	{
		m_fAnimeFrame = min(m_fAnimeFrame, m_AnimeScene.iEndFrame);
		m_fAnimeFrame = max(m_fAnimeFrame, m_AnimeScene.iStartFrame);
		m_fAnimeInverse *= -1.0f;
	}

	//for (int iBone = 0; iBone < m_pObjectListS.size(); iBone++)
	//{
	//	K_Matrix matAnim = m_pObjectListS[iBone]->InterPlate(m_fAnimeFrame, m_AnimeScene);
	//	m_cbDataBone.matBone[iBone] = matAnim.Transpose();
	//}

	// 요 밑에 덩어리들 이해를 못하겠음... 나중에 시간날때 다시보던가 말던가.. 왜 콘스트 버퍼를 각자쓰는겨? 
	// object + skinning
	std::vector<K_Matrix> matCurrentAnimList;
	for (int iBone = 0; iBone < m_pObjectListS.size(); iBone++)
	{
		K_Matrix matAnimation = m_pObjectListS[iBone]->InterPlate(m_fAnimeFrame, m_AnimeScene);
		m_cbDataBone.matBone[iBone] = matAnimation.Transpose();
		matCurrentAnimList.push_back(matAnimation);
	}
	pContext->UpdateSubresource(m_pConstantBufferBone, 0, nullptr, &m_cbDataBone, 0, 0);

	// skinning
	for (int iDraw = 0; iDraw < m_pDrawObjListS.size(); iDraw++)
	{
		if (m_pDrawObjListS[iDraw]->m_dxMatrixBindPseMap.size()) // skinning 있는놈만 이밑에걸 돌려라 같은디
		{
				for (int iBone = 0; iBone < m_pObjectListS.size(); iBone++)
				{
					auto iter = m_pDrawObjListS[iDraw]->m_dxMatrixBindPseMap.find(iBone);
					if (iter != m_pDrawObjListS[iDraw]->m_dxMatrixBindPseMap.end())
					{
						K_Matrix matBind = iter->second;
						K_Matrix matAnim = matBind * matCurrentAnimList[iBone];
						m_cbDataBone.matBone[iBone] = matAnim.Transpose();
					}
				}
				pContext->UpdateSubresource(m_pDrawObjListS[iDraw]->m_pConstantBufferBone, 0, nullptr, &m_cbDataBone, 0, 0);
				
		}
	}

	return true;
}

bool K_FBXLoader::ParseMeshSkinning(FbxMesh* pFbxMesh, K_FbxObjectSkinning* pObject)
{
	// 리깅 도구( 뼈대에 스킨을 붙이는 작업도구)
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0) return false;

	// iNumVertex == 메쉬의 정점 개수와 동일해야 한다.
	int iNumVertex = pFbxMesh->GetControlPointsCount();
	pObject->m_IndexWeightList.resize(iNumVertex);

	for (int iDeformer = 0; iDeformer < iDeformerCount; iDeformer++)
	{
		FbxDeformer* deformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
		FbxSkin* pSkin = (FbxSkin*)deformer;
		// 뼈대가 영향을 미치는 정점 덩어리
		int iNumClusterCount = pSkin->GetClusterCount();
		for (int iCluster = 0; iCluster < iNumClusterCount; iCluster++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iCluster);
			FbxNode* pFbxNode = pCluster->GetLink();

			if (m_pObjectIDMap.find(pFbxNode) == m_pObjectIDMap.end()) 
				return false; // 난 왜 밑에서 뻑나지..? 
			int iBoneIndex = m_pObjectIDMap.find(pFbxNode)->second;

			// 뼈대공간으로 변환하는 행렬이 필요하다.
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPostion;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPostion);
			FbxAMatrix matBindPos = matReferenceGlobalInitPostion.Inverse() * matXBindPose;

			// 선생님 코드에서는 일단 TM으로 받아오고 역행렬로 만드는데, 내껀 없으니까 일단 이렇게 해봄
			matBindPos = matBindPos.Inverse(); 
			K_Matrix matInvBindPos = ConvertMatrix(matBindPos);
			pObject->m_dxMatrixBindPseMap.insert(std::make_pair(iBoneIndex, matInvBindPos));

			int iNumWeightCounter = pCluster->GetControlPointIndicesCount(); // 임의의 1개 정점에 영향을 미치는 뼈대의 개수
			int* pIndicss = pCluster->GetControlPointIndices(); // 정점에 영향을 미치는 행렬?뼈대? 의 인덱스
			double* pWeights = pCluster->GetControlPointWeights(); // 가중치, 바로 위엣놈이 얼마나 영향을 미치는지
			for (int iVertex = 0; iVertex < iNumWeightCounter; iVertex++)
			{
				int iVertexIndex = pIndicss[iVertex];
				float fWeight = pWeights[iVertex];
				pObject->m_IndexWeightList[iVertexIndex].insert(iBoneIndex, fWeight);
			}
		}
	}
	return true;
}
