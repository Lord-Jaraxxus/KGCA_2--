enum Color { RED, BLACK, DOUBLE_BLACK };

class Node //������ Ʈ�� ����, for���� �Ἥ �� ��ũ�帮��Ʈ �׷��� ��Ÿ�� �����ְ� ��������
{
public:
	int m_iData;
	int m_iIndex;
	//int m_iDepth;
	int m_iColor;

	Node* m_pParent;
	Node* m_pChild[2];

	Node() : m_iData(0), m_iIndex(0), m_iColor(RED) //m_iDepth(0)
	{
		m_pParent = nullptr;
		m_pChild[0] = nullptr;
		m_pChild[1] = nullptr;
	}

	~Node() 
	{
	}
};

