class Node //당장은 트리 전용, for문을 써서 뭐 링크드리스트 그래프 기타등등도 쓸수있게 변형가능
{
public:
	int m_iData;
	int m_iIndex;
	//int m_iDepth;
	Node* m_pParent;

	Node* m_pChild[2];

	Node() : m_iData(0), m_iIndex(0) //m_iDepth(0)
	{
		m_pParent = nullptr;
		m_pChild[0] = nullptr;
		m_pChild[1] = nullptr;
	}

	~Node() 
	{
	}
};

