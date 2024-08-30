
//--------------------------------------------------------------------------------------------------
// ++ Author: PJ81
// ++ Date: 28.08.2024
//--------------------------------------------------------------------------------------------------
#include "../inc/stdafx.h"
#include "../inc/cParticleLife.h"
#include "../inc/misc.h"

//--------------------------------------------------------------------------------------------------
void cParticleLife::RenderFrame(void) {

    FillRect(m_hBackDC, &m_Rect, m_BlackBrush);

    UpdateParticles();

    // draw particles
    cParticle p;
    for (int x = 0; x < MAX_POP; x++) {
        p = m_Particles[x];
        BitBlt(m_hBackDC, (int)p.pos.x, (int)p.pos.y, 3, 3, m_hDCs[p.type], 0, 0, SRCCOPY);
    }

    SHORT space = GetAsyncKeyState(VK_SPACE);
    if ((1 << 15) & space) {
        CreateRules();
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::UpdateParticles() {
    cParticle* pj, *pi;
    float d;
    cBehavior behavior;
    cVector dir, force, totForce;

    for (int i = 0; i < MAX_POP; i++) {
        pi = &m_Particles[i];

        for (int j = 0; j < MAX_POP; j++) {
            if (j == i) continue;

            pj = &m_Particles[j];

            behavior = m_Rules[pi->type][pj->type];

            dir = pj->pos - pi->pos;
            
            dir.x += dir.x > m_ihWid ? -m_iWndWid : dir.x < -m_ihWid ? m_iWndWid : 0;
            dir.y += dir.y > m_ihHei ? -m_iWndHei : dir.y < -m_ihHei ? m_iWndHei : 0;

            d = dir.length();
            dir.normalize();

            if (d < behavior.attract.radius) {
                force = dir * behavior.attract.force;
                force *= .05f;
                totForce += force;
            }

            if (d < behavior.repel.radius) {
                force = dir * behavior.repel.force*3;
                force *= .05f;
                totForce += force;
            }
        }
        
        totForce *= .85f;
        pi->vel += totForce;
        pi->pos += pi->vel;
        pi->vel *= .5f;

        pi->pos.x = (float)fmod((pi->pos.x + m_iWndWid), m_iWndWid);
        pi->pos.y = (float)fmod((pi->pos.y + m_iWndHei), m_iWndHei);
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::CreateParticles(void) {
    for (int x = 0; x < MAX_POP; x++) {
        m_Particles[x].type = rand() % MAX_TYPES;
        m_Particles[x].pos.set((float)(rand() % m_iWndWid), (float)(rand() % m_iWndHei));
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::CreateRules(void) {
    for (int y = 0; y < MAX_TYPES; y++) {
        for (int x = 0; x < MAX_TYPES; x++) {

            m_Rules[x][y].attract.radius = m_Rand.randF(70.f, 150.f);
            m_Rules[x][y].attract.force = m_Rand.randF(.3f, 1.f);
            if (m_Rand.randF() < .5) m_Rules[x][y].attract.force = -m_Rules[x][y].attract.force;

            m_Rules[x][y].repel.radius = m_Rand.randF(5.f, 25.f);
            m_Rules[x][y].repel.force = -m_Rand.randF(.3f, 1.f);
        }
    }
}
//--------------------------------------------------------------------------------------------------
void cParticleLife::Initialize() {

    for (int x = 0; x < 6; x++) {
        m_hBits[x] = LoadImage(m_hInst, MAKEINTRESOURCE(x + IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
        m_hDCs[x] = CreateCompatibleDC(0);

        SelectObject(m_hDCs[x], m_hBits[x]);
    }

    CreateParticles();

    CreateRules();

    m_ihWid = m_iWndWid / 2;
    m_ihHei = m_iWndHei / 2;

    m_BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    m_Rect = { 0, 0, m_iWndWid, m_iWndHei };
}
//--------------------------------------------------------------------------------------------------