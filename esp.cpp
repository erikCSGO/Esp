//auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
//bool is_local_player = pEnt == local_player;
//bool is_teammate = local_player->GetTeam() == pEnt->GetTeam() && !is_local_player;

void ESP::RenderBox(C_BaseEntity* pEnt)
{

    Vector vecScreenOrigin, vecOrigin = pEnt->GetRenderOrigin();
    if (!Utils::WorldToScreen(vecOrigin, vecScreenOrigin))
        return;

    Vector vecScreenBottom, vecBottom = vecOrigin;
    vecBottom.z += (pEnt->GetFlags() & FL_DUCKING) ? 54.f : 72.f;
    if (!Utils::WorldToScreen(vecBottom, vecScreenBottom))
        return;


    const auto sx = int(std::roundf(vecScreenOrigin.x)),
               sy = int(std::roundf(vecScreenOrigin.y)),
               h  = int(std::roundf(vecScreenBottom.y - vecScreenOrigin.y)),
               w  = int(std::roundf(h * 0.25f));

    
    g_Render.Rect(sx - w, sy, sx + w, sy + h, (pEnt->GetTeam() == localTeam) ? teamColor : enemyColor);

    
    g_Render.Rect(sx - w - 1, sy - 1, sx + w + 1, sy + h + 1, Color::Black());
    g_Render.Rect(sx - w + 1, sy + 1, sx + w - 1, sy + h - 1, Color::Black());
}

void ESP::RenderHealth(C_BaseEntity* pEnt)
{
	Vector vecScreenOrigin, vecOrigin = pEnt->GetRenderOrigin();
	if (!Utils::WorldToScreen(vecOrigin, vecScreenOrigin))
		return;

	Vector vecScreenBottom, vecBottom = vecOrigin;
	vecBottom.z += (pEnt->GetFlags() & FL_DUCKING) ? 54.f : 72.f;
	if (!Utils::WorldToScreen(vecBottom, vecScreenBottom))
		return;

	const auto sx = int(std::roundf(vecScreenOrigin.x)),
		sy = int(std::roundf(vecScreenOrigin.y)),
		h = int(std::roundf(vecScreenBottom.y - vecScreenOrigin.y)),
		w = int(std::roundf(h * 0.25f));

	int enemyHp = pEnt->GetHealth();

	std::string s = std::to_string(enemyHp);
	char const *pchar = s.c_str();


	if (pEnt->GetHealth() <= 20)
	{
		g_Render.String(sx + w - 13, sy + h, CD3DFONT_CENTERED_Y | CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW, Color(190, 0, 0, 255), g_Fonts.pFontnasalizationrg.get(), pchar);
	}
	else if (pEnt->GetHealth() > 50)
	{
		g_Render.String(sx + w - 13, sy + h, CD3DFONT_CENTERED_Y | CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW, Color(255, 255, 255, 255), g_Fonts.pFontnasalizationrg.get(), pchar);
	}
	else if (pEnt->GetHealth() >= 20 && pEnt->GetHealth() <= 50)
	{
		g_Render.String(sx + w - 13, sy + h, CD3DFONT_CENTERED_Y | CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW, Color(230, 230, 0, 255), g_Fonts.pFontnasalizationrg.get(), pchar);
	}
}

void ESP::RenderName(C_BaseEntity* pEnt, int iterator)
{
    Vector vecScreenOrigin, vecOrigin = pEnt->GetRenderOrigin();
    if (!Utils::WorldToScreen(vecOrigin, vecScreenOrigin))
        return;

    Vector vecScreenBottom, vecBottom = vecOrigin;
    vecBottom.z += (pEnt->GetFlags() & FL_DUCKING) ? 54.f : 72.f;
    if (!Utils::WorldToScreen(vecBottom, vecScreenBottom))
        return;


    PlayerInfo_t pInfo;
    g_pEngine->GetPlayerInfo(iterator, &pInfo);

    int sx = std::roundf(vecScreenOrigin.x);
    int sy = std::roundf(vecScreenOrigin.y);
    int h = std::roundf(vecScreenBottom.y - vecScreenOrigin.y);

    g_Render.String(sx, sy + h - 16, CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW,
                    Color(255,255,255,255),
                    g_Fonts.pFontnasalizationrg.get(), pInfo.szName);
}

void ESP::RenderWeaponName(C_BaseEntity* pEnt)
{
    Vector vecScreenPos;
    if (!Utils::WorldToScreen(pEnt->GetRenderOrigin(), vecScreenPos))
        return;

    auto weapon = pEnt->GetActiveWeapon();

    if (!weapon)
        return;

    auto strWeaponName = weapon->GetName();

    strWeaponName.erase(0, 7);
    std::transform(strWeaponName.begin(), strWeaponName.end(), strWeaponName.begin(), ::toupper);

    g_Render.String(vecScreenPos.x, vecScreenPos.y, CD3DFONT_CENTERED_X | CD3DFONT_DROPSHADOW,
		Color(255, 255, 255, 255),
		g_Fonts.pFontnasalizationrg.get(), strWeaponName.c_str());
}


void ESP::Indicators(C_BaseEntity* pEnt)
{
	auto nci = g_pEngine->GetNetChannelInfo();
	
	std::string ping = std::to_string((int)(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING) * 1000)) + "ms";
	if (nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING) * 1000 <= 80)
	{
		g_Render.String(15, 1000, CD3DFONT_DROPSHADOW, Color(0, 190, 0, 255), g_Fonts.pFontnasalizationrg.get(), (ping).c_str());
	}
	else
	{
		g_Render.String(15, 1000, CD3DFONT_DROPSHADOW, Color(190, 0, 0, 255), g_Fonts.pFontnasalizationrg.get(), (ping).c_str());
	}

	//int ticknibba = g_pEngine->GetServerTick();

	std::string fpsxd = std::to_string(get_fps());
	if(get_fps() > 63) {
		g_Render.String(15, 990, CD3DFONT_DROPSHADOW, Color(0, 190, 0, 255), g_Fonts.pFontnasalizationrg.get(), ("Frames: " + fpsxd).c_str());
	}

	if(get_fps() < 64)
	{
		g_Render.String(15, 990, CD3DFONT_DROPSHADOW, Color(190, 0, 0, 255), g_Fonts.pFontnasalizationrg.get(), ("Frames: "  + fpsxd).c_str());
	}

}

/*bool ESP::Filter(C_BaseEntity *pEnt)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	bool is_local_player = pEnt == local_player;
	bool is_teammate = local_player->GetTeam() == pEnt->GetTeam() && !is_local_player;

	if (!is_teammate)
	{

	}
	else if (is_teammate)
	{

	}

	if (!pEnt || !local_player) return true;
	if (!pEnt->IsAlive()) return true;
	if (pEnt->GetTeam() == local_player->GetTeam() && !g_Settings.bTeamEsp) return true;
	if (pEnt->GetTeam() != 2 && pEnt->GetTeam() != 3) return true;

	return false;
}
*/
/*void ESP::ViewModelFOV(float& fov)
{
	if (g_Settings.bFovChanger)
	{
		C_BaseEntity* pPlayer = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->GetLifeState())
		{

			if (!pPlayer)
				return;
		}

		fov = (float)ESP::FovModelView;
	}
}
*/

void ESP::Render()
{
    if (!g::pLocalEntity || !g_pEngine->IsInGame())
        return;

    localTeam = g::pLocalEntity->GetTeam();


    for (int it = 1; it <= g_pEngine->GetMaxClients(); ++it)
    {
        C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(it);


        if (!pPlayerEntity
            || !pPlayerEntity->IsAlive()
            || pPlayerEntity->IsDormant()
            || pPlayerEntity == g::pLocalEntity)
            continue;

		//if (!g_Settings.bTeamEsp


        if (g_Settings.bShowBoxes)
            this->RenderBox(pPlayerEntity);

		if (g_Settings.bShowHealth)
			this->RenderHealth(pPlayerEntity);

        if (g_Settings.bShowNames)
            this->RenderName(pPlayerEntity, it);

        if (g_Settings.bShowWeapons)
            this->RenderWeaponName(pPlayerEntity);

		if (g_Settings.bIndicators)
			this->Indicators(pPlayerEntity);
    }
}
