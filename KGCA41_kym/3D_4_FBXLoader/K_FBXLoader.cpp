#include "K_FBXLoader.h"

bool K_FBXLoader::Init()
{
    m_pFbxManager = FbxManager::Create();
    m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
    m_pFbxScene = FbxScene::Create(m_pFbxManager, "");

	// ����
	FbxSystemUnit::cm.ConvertScene(m_pFbxScene);
	// ����(���)
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
	for (auto obj : m_pDrawObjList)
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
	PreProcess(m_pRootNode);

	return true;
}

// fbx Ʈ���� �� ���鼭 ��带 FbxObject�� ��ƿ�
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

// �޽� �м�, �츮���� �ʿ��� ������(���ؽ�����, �ؽ�ó�̸�, �ؽ�ó UV���) �����
void K_FBXLoader::ParseMesh(FbxMesh* pFbxMesh, K_FbxObject* pObject)
{
	FbxNode* pNode = pFbxMesh->GetNode();

	FbxAMatrix geom; // ����(��Į)���(�ʱ� ���� ��ġ�� ��ȯ�� �� ����Ѵ�.)
	FbxVector4 trans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalLocalMatrix = geom;
	normalLocalMatrix = normalLocalMatrix.Inverse();
	normalLocalMatrix = normalLocalMatrix.Transpose();

	//// �������
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

	// Layer ����
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
		// 24 �̻��� ������ �ִ�.
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
	if (iNumMtrl < 1) // ���� ���׸���, �� �ؽ�ó�� �ϳ��� ���� ���
	{
		pObject->m_vVertexDataListList.resize(1);
		pObject->m_vTextureNameList.resize(1);
		pObject->m_vTextureNameList[0] = L"Default.jpg"; // �ϴ� �ӽ÷� ���� ����� ������
	}


	int iNumPolyCount = pFbxMesh->GetPolygonCount();
	// 3 ���� -> 1������( �ﰢ��)
	// 4 ���� -> 1������( ���� )
	int iNumFace = 0;
	int iBasePolyIndex = 0;
	int iSubMtrl = 0;

	// ������
	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints();
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;

		// ��� �� ���� ���° sm���� �˷��ִ� �ž� �ƴϸ� �� sm�� ����� �˷��ִ°ž�? 
		if(MaterialSet) iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet);

		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// ���� �ε���
			int iCornerIndex[3];
			iCornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, 0);
			iCornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 2);
			iCornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, iFace + 1);

			// �ؽ�ó UV �ε���������,��� �� ���� �ִ°�?
			int iUVIndex[3];
			iUVIndex[0] = pFbxMesh->GetTextureUVIndex(iPoly, 0);
			iUVIndex[1] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 2);
			iUVIndex[2] = pFbxMesh->GetTextureUVIndex(iPoly, iFace + 1);

			// �����÷��ε���
			int VertexColor[3] = { 0, iFace + 2, iFace + 1 };

			// �ƹ�ư for�� ���鼭 �ʿ��� ������ ����
			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				// p, position (���ؽ� ����)
				int vertexID = iCornerIndex[iIndex];
				FbxVector4 v2 = pVertexPositions[vertexID];
				PNCT_VERTEX tVertex;
				FbxVector4 v = geom.MultT(v2);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];

				// n, normal (���� �븻)
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

				// c, color (���� �÷�)
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

				//  t, texture (�ؽ�ó UV��ǥ)
				if (VertexUVSet) 
				{
					FbxVector2 t = ReadTextureCoord(pFbxMesh, VertexUVSet, iCornerIndex[iIndex], iUVIndex[iIndex]);
					tVertex.t.x = t.mData[0];
					tVertex.t.y = 1.0f - t.mData[1];
				}
				pObject->m_vVertexDataListList[iSubMtrl].push_back(tVertex); // �̰� �׳� �̷��� �����ص�?
			}
		}
		iBasePolyIndex += iPolySize;
	}

	m_pDrawObjList.push_back(pObject);
}

FbxVector2 K_FBXLoader::ReadTextureCoord(FbxMesh* pFbxMesh, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex)
{
	FbxVector2 t;

	/*enum EMappingMode
	{
		eNone,				// ������ �������� �ʾҴ�.
		eByControlPoint,	// ������ �� ������ 1���� ������ �Ǿ� �ִ�.
		eByPolygonVertex,	// �� ������ ���� �������� ���ο� ��ǥ�� �ִ�.
		eByPolygon,         // ��ü �����￡ ���� ��ǥ�� �ִ�.
		eByEdge,			// ������ 1���� ���� ��ǥ�� �ִ�.
		eAllSame			// ��ü �����￡ 1���� ���� ��ǥ�� �ִ�.
	};*/

	//enum EReferenceMode
	//{
	//	eDirect,   // n��° ���������� mDirectArray�� n��° ��ġ�� �ִ�.
	//	eIndex,    // fbx 5.0 ���� �������� ���Ǿ�����. �̻� ���������� eIndexToDirect�� ��ü�Ǿ���.
	//	eIndexToDirect
	//};


	// �ؽ�ó ���� ����� ����?
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
		// �ΰ��� ������ ó��, ���� �����ϱ� �󰡸� �ؽ�ó�� �񽺵��ϰ� �پ��ٰ� ī�����
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
	// ���ι��
	//eNone,
	//eByControlPoint,  // ������
	//eByPolygonVertex, //  
	//eByPolygon, // �����︶�� �ٸ��� �ִ�.
	//eAllSame - ��üǥ�鿡 1���� ������ǥ�� �ִ�.

	int iSubMtrl = 0;
	if (pMaterialSetList != nullptr)
	{
		switch (pMaterialSetList->GetMappingMode())
		{
		case FbxLayerElement::eByPolygon:
		{
			// ���� ������ �迭�� ����Ǵ� ���
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

void K_FBXLoader::LoadAnimation(K_FbxObject* pObject)
{
	FbxNode* pNode = pObject->m_pFbxNode;
	FbxAnimStack* stackAnim = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	FbxLongLong s = 0;
	FbxLongLong n = 0;
	FbxTime::EMode TimeMode;

	if (stackAnim) // ������ �� ����־ ���� �ʴ�? �� �ٵ� �׷��� �ִϸ��̼� ���� ������Ʈ�鵵 ����� ���޾ư���
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

	pObject->m_AnimeScene.iStartFrame = s;
	pObject->m_AnimeScene.iEndFrame = n;
	pObject->m_AnimeScene.fFrameSpeed = 30.0f;
	pObject->m_AnimeScene.fTickPerFrame = 160;
	FbxTime time;
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		K_AnimeTrack track;
		track.iFrame = t;
		FbxAMatrix fbxMatrix = pNode->EvaluateGlobalTransform(time);
		track.matAnime = ConvertMatrix(fbxMatrix);
		TBASIS_EX::TMatrix TM = ConvertMatrixKtoT(track.matAnime);
		TBASIS_EX::D3DXMatrixDecompose(&track.s, &track.r, &track.t, &TM); // ���� ����?����� srt�� �ɰ�
		pObject->m_AnimeTracks.push_back(track);
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
