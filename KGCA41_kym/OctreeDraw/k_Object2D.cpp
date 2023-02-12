#include "k_Object2D.h"

void k_Object2D::SetPosition(float x, float y, float w, float h)
{
    m_sRect.Set(x, y, w, h);

    float hw = w / 2.0f;
    float hh = h / 2.0f;
    float r1 = sqrt(hw * hw + hh * hh);
    m_sCir.Set(x + hw, y + hh, r1);

    float x1 = (m_sRect.x1 - 50.0f) / 50.0f;
    float y1 = (m_sRect.y1 - 50.0f) / 50.0f;
    float x2 = (m_sRect.x2 - 50.0f) / 50.0f;
    float y2 = (m_sRect.y2 - 50.0f) / 50.0f;

    k_Vector Vertex1 = { x1, y1, 0.0f };
    k_Vector Vertex2 = { x2, y1, 0.0f };
    k_Vector Vertex3 = { x1, y2, 0.0f };
    k_Vector Vertex4 = { x2, y2 ,0.0f };

    m_VertexList.clear();
    m_VertexList.resize(6);
    m_VertexList[0] = { Vertex1, {0.0f, 1.0f, 1.0f, 0.0f} };
    m_VertexList[1] = { Vertex3, {0.0f, 1.0f, 1.0f, 0.0f} };
    m_VertexList[2] = { Vertex2, {0.0f, 1.0f, 1.0f, 0.0f} };
    m_VertexList[3] = { Vertex2, {0.0f, 1.0f, 1.0f, 0.0f} };
    m_VertexList[4] = { Vertex3, {0.0f, 1.0f, 1.0f, 0.0f} };
    m_VertexList[5] = { Vertex4, {0.0f, 1.0f, 1.0f, 0.0f} };
}

void k_Object2D::SetForces(float f)
{
    m_vForces = m_vDirection * f;
}

void k_Object2D::SetVector(k_Vector2D& v, float x, float y)
{
    v.x = x;
    v.y = y;
}

void k_Object2D::AddForces(k_Vector2D f)
{
    //m_vForces += f;
}

k_Object2D::k_Object2D()
{
    SetPosition
    (
        (rand() % 90),
        (rand() % 90),
        5.0f + (rand() % 5),
        5.0f + (rand() % 5)
    );
    m_vDirection.Set(1.0f + rand() % 9, 1.0f + rand() % 9);
    SetForces(1.0f);
    m_vVelocity.Set(0.0f, 0.0f);
    m_vAcceleration.Set(0.0f, 0.0f);
    m_fMass = 1.0f;
    m_fFriction = 1.0f;
}
