/*******************************************************************************
Copyright (c) 2010, Jonathan Hiller (Cornell University)
If used in publication cite "J. Hiller and H. Lipson "Dynamic Simulation of Soft Heterogeneous Objects" In press. (2011)"

This file is part of Voxelyze.
Voxelyze is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Voxelyze is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/

#include "VX_SimGA.h"
#include <algorithm>    // std::max

CVX_SimGA::CVX_SimGA()
{
	Fitness = 0.0f;
	TrackVoxel = 0;
	FitnessFileName = "";
//	print_scrn = false;
	WriteFitnessFile = false;
	FitnessType = FT_NONE;	//no reporting is default

}

void CVX_SimGA::SaveResultFile(std::string filename)
{
	CXML_Rip XML;
	WriteResultFile(&XML);
	XML.SaveFile(filename);
}


void CVX_SimGA::WriteResultFile(CXML_Rip* pXML)
{

	// std::cout << "here1" << std::endl;
	vfloat cageCenter = 0.0;
	int nVox = NumVox();
	// std::cout << "NumVox: " << nVox << std::endl;
	for (int i=0; i<nVox; i++){
		CVXS_Voxel* it = &VoxArray[i];
		if (it->GetMaterialIndex() == 5)
		{
			// std::cout <<  i << ": " << fabs(it->GetCurPos().x) <<  ", " << fabs(it->GetCurPos().x) << ", " << std::max(fabs(it->GetCurPos().x), fabs(it->GetCurPos().y)) << std::endl;
			if (std::max(fabs(it->GetCurPos().x), fabs(it->GetCurPos().y)) > cageCenter)
			{
				cageCenter = std::max(fabs(it->GetCurPos().x), fabs(it->GetCurPos().y));
				// std::cout << "new best!" << std::endl;
			}
		}
	}
	cageCenter = cageCenter/2.0;


	vfloat bestDist = 0.0;
	// int nVox = NumVox();
	// std::cout << "NumVox: " << nVox << std::endl;
	for (int i=0; i<nVox; i++){
		CVXS_Voxel* it = &VoxArray[i];
		if (it->GetMaterialIndex() != 5 and it->GetMaterialIndex() != 0 )
		{
			// std::cout <<  i << ": " << fabs(it->GetCurPos().x) <<  ", " << fabs(it->GetCurPos().x) << ", " << std::max(fabs(it->GetCurPos().x), fabs(it->GetCurPos().y)) << std::endl;
			if (std::max(fabs(it->GetCurPos().x-cageCenter), fabs(it->GetCurPos().y-cageCenter)) > bestDist)
			{
				bestDist = std::max(fabs(it->GetCurPos().x-cageCenter), fabs(it->GetCurPos().y-cageCenter));
				// std::cout << "new best!" << std::endl;
			}
		}
	}

	int numOutside = 0;
	for (int i=0; i<nVox; i++){
		CVXS_Voxel* it = &VoxArray[i];
		if (it->GetMaterialIndex() != 5 and it->GetMaterialIndex() != 0 )
		{
			// std::cout <<  i << ": " << fabs(it->GetCurPos().x) <<  ", " << fabs(it->GetCurPos().x) << ", " << std::max(fabs(it->GetCurPos().x), fabs(it->GetCurPos().y)) << std::endl;
			if ( it->GetCurPos().x < minCageX or it->GetCurPos().y < minCageY or it->GetCurPos().x > maxCageX or it->GetCurPos().y > maxCageY)
			{
				numOutside++;
			}
		}
	}

	// std::cout << "here2" << std::endl;

	pXML->DownLevel("Voxelyze_Sim_Result");
	pXML->SetElAttribute("Version", "1.0");
	pXML->DownLevel("Fitness");
	// pXML->Element("FinalCOM_Dist", SS.CurCM.Normalize()-IniCM.Normalize());
	// pXML->Element("FinalCOM_DistX", SS.CurCM.x-IniCM.x);
	// pXML->Element("FinalCOM_DistY", SS.CurCM.y-IniCM.y);
	// pXML->Element("FinalCOM_DistZ", SS.CurCM.z-IniCM.z);
	pXML->Element("EndPosX", SS.CurCM.x-cageCenter);
	pXML->Element("EndPosY", SS.CurCM.x-cageCenter);
	// pXML->Element("FinalFitness", bestDist);
	pXML->Element("FinalFitness", numOutside);
	pXML->UpLevel();
	pXML->UpLevel();

	// std::cout << "here3" << std::endl;

}

void CVX_SimGA::WriteAdditionalSimXML(CXML_Rip* pXML)
{
	pXML->DownLevel("GA");
		pXML->Element("Fitness", Fitness);
		pXML->Element("FitnessType", (int)FitnessType);
		pXML->Element("TrackVoxel", TrackVoxel);
		pXML->Element("FitnessFileName", FitnessFileName);
		pXML->Element("WriteFitnessFile", WriteFitnessFile);
	pXML->UpLevel();
}

bool CVX_SimGA::ReadAdditionalSimXML(CXML_Rip* pXML, std::string* RetMessage)
{
	if (pXML->FindElement("GA")){
		int TmpInt;
		if (!pXML->FindLoadElement("Fitness", &Fitness)) Fitness = 0;
		if (pXML->FindLoadElement("FitnessType", &TmpInt)) FitnessType=(FitnessTypes)TmpInt; else Fitness = 0;
		if (!pXML->FindLoadElement("TrackVoxel", &TrackVoxel)) TrackVoxel = 0;
		if (!pXML->FindLoadElement("FitnessFileName", &FitnessFileName)) FitnessFileName = "";
		if (!pXML->FindLoadElement("WriteFitnessFile", &WriteFitnessFile)) WriteFitnessFile = true;
		pXML->UpLevel();
	}

	return true;
}