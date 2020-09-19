#include "stdafx.h"
#include "2DViewMaths.h"

// Hiden functions for 2DViewMaths
static void whichShadowsAreOnObject(Poligon& in, Poligon& ck, map<double, ShadowOverlapAngle>& setShadows)
{
	// If there are objects behind OURS, OURS is visible
	if (ck.rad < in.rad)
	{
		ShadowOverlapAngle shadow = { 0.0, 360.0, 0.0 };

		//if there is object which is OUT of OUR shadow on RIGHT, it's no problem for us
		if ((ck.A - in.A) > 0 &&
			(ck.A - in.B) > 0)
		{
			return;
		}
		//if there is object which is OUT of OUR shadow on LEFT, it's no problem for us
		else if ((ck.B - in.A) < 0 &&
			(ck.B - in.B) < 0)
		{
			return;
		}
		else if ((ck.A - in.A) >= 0 &&
			(ck.B - in.B) >= 0)
		{
			shadow.Z = ck.A;
			shadow.W = in.B;
		}
		else if ((ck.A - in.A) <= 0 &&
			(ck.B - in.B) >= 0)
		{
			shadow.Z = in.A;
			shadow.W = in.B;
		}
		else if ((ck.A - in.A) <= 0 &&
			(ck.B - in.B) <= 0)
		{
			shadow.Z = in.A;
			shadow.W = ck.B;
		}
		else if ((ck.A - in.A) >= 0 &&
			(ck.B - in.B) <= 0)
		{
			shadow.Z = ck.A;
			shadow.W = ck.B;
		}

		shadow.zRaper = shadow.Z;

		std::map<double, ShadowOverlapAngle>::iterator it;
		it = setShadows.find(shadow.zRaper);
		if (it != setShadows.end())
		{
			//check if there is a shadow in the SET bigger than ours
			if ((shadow.W - shadow.Z) >
				(it->second.W - it->second.Z))
				setShadows.erase(it);
		}
		setShadows.insert(std::pair<double, ShadowOverlapAngle>(shadow.zRaper, shadow));
	}
}

static bool ifObjectIsVisible(Poligon& in, vector<ShadowOverlapAngle> loopList)
{
	ShadowOverlapAngle tmpShadow = { 0.0, 0.0, 0.0 };

	if (!loopList.size())
	{
		in.isVisible = true;
		return true;
	}

	tmpShadow.Z = loopList[0].Z;
	tmpShadow.W = loopList[0].W;

	for (size_t i = 1; i < loopList.size(); i++)
	{
		/*OK New*/
		if ((loopList[i].Z - tmpShadow.Z) > 0 &&
			(loopList[i].Z - tmpShadow.W) > 0)
		{
			in.isVisible = true;
			return true;
			//continue;
		}
		/*OK 1*/if ((loopList[i].Z - tmpShadow.Z) >= 0 &&
			(loopList[i].W - tmpShadow.W) >= 0)
		{
			tmpShadow.W = loopList[i].W;
		}
		/*OK 4*/else if ((loopList[i].Z - tmpShadow.Z) >= 0 &&
			(loopList[i].W - tmpShadow.W) <= 0)
		{
			//next iteration;
		}


	}

	if ((tmpShadow.Z - in.A) <= 0 &&
		(tmpShadow.W - in.B) >= 0)
	{
		in.isVisible = false;
		return false;
	}
	else
	{
		in.isVisible = true;
		return true;
	}
}

static bool sortShadowsCheckVisibility(Poligon& in, map<double, ShadowOverlapAngle>& setShadows)
{
	vector<ShadowOverlapAngle> sortedShadowList;

	for (auto& x : setShadows) {
		//cout << x.first << ": " << x.second.averageShadowAngle << endl;
		sortedShadowList.push_back(x.second);
	}

	return ifObjectIsVisible(in, sortedShadowList);
}

static void aroundRoomCheck(vector<Poligon> olist, vector<Poligon>& outResult)
{
	map<double, ShadowOverlapAngle> _shadowsOnOurObject;

	for (size_t i = 0; i < olist.size(); i++)
	{
		for (size_t j = 0; j < olist.size(); j++)
		{
			if (i == j) {}
			else
			{
				whichShadowsAreOnObject(olist[i], olist[j], _shadowsOnOurObject);
			}
		}
		sortShadowsCheckVisibility(olist[i], _shadowsOnOurObject);
		outResult.push_back(olist[i]);
		_shadowsOnOurObject.clear();
	}

	//return true;
}

void PanoramaView::perspective(FileOperation& entrData)
{
	aroundRoomCheck(entrData.surroundingObjects.matrixOfPoligons, entrData.result);
}
