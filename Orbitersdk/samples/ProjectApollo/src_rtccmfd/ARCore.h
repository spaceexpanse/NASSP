#ifndef __ARCORE_H
#define __ARCORE_H

#include "Orbitersdk.h"
#include "MFDButtonPage.hpp"
#include "ApollomfdButtons.h"
#include "OrbMech.h"
#include "EntryCalculations.h"
#include "soundlib.h"
#include "apolloguidance.h"
#include "dsky.h"
#include "csmcomputer.h"
#include "saturn.h"
#include "mcc.h"
#include "rtcc.h"
#include "LunarTargetingProgram.h"
#include <queue>

struct ApolloRTCCMFDData {  // global data storage
	int connStatus;
	int emem[24];
	int uplinkState;
	std::queue<unsigned char> uplinkBuffer;
	double uplinkBufferSimt;
};

class AR_GCore
{
public:
	AR_GCore(VESSEL* v);
	~AR_GCore();

	void SetMissionSpecificParameters();
	void MPTMassUpdate();
	int MPTTrajectoryUpdate(VESSEL *ves, bool csm);

	bool MissionPlanningActive;
	int mission;				//0=manual, 7 = Apollo 7, 8 = Apollo 8, 9 = Apollo 9, etc.

	VESSEL *pMPTVessel;
	int MPTVesselNumber;

	int mptInitError;

	RTCC* rtcc;
};

class ARCore {
public:
	ARCore(VESSEL* v, AR_GCore* gcin);
	~ARCore();
	void lambertcalc();
	void SPQcalc();
	void GPMPCalc();
	void REFSMMATCalc();
	void SkylabCalc();
	void LunarLaunchTargetingCalc();
	void LDPPalc();
	void LunarLiftoffCalc();
	void LOICalc();
	void LmkCalc();
	void EntryCalc();
	void DeorbitCalc();
	void TLCCCalc();
	void EntryUpdateCalc();
	void StateVectorCalc(int type);
	void AGSStateVectorCalc();
	void LandingSiteUpdate();
	void CSMLSUplinkCalc();
	void LMLSUplinkCalc();
	void CSMLandingSiteUplink();
	void LMLandingSiteUplink();
	void VecPointCalc();
	void TerrainModelCalc();
	void DKICalc();
	void LAPCalc();
	void DAPPADCalc();
	void AscentPADCalc();
	void PDAPCalc();
	void CycleFIDOOrbitDigitals1();
	void CycleFIDOOrbitDigitals2();
	void CycleSpaceDigitals();
	void CycleVectorPanelSummary();
	void SpaceDigitalsMSKRequest();
	void CycleNextStationContactsDisplay();
	void RecoveryTargetSelectionCalc();
	void RTETradeoffDisplayCalc();
	void GetAGSKFactor();
	void GeneralMEDRequest();
	void SkylabSaturnIBLaunchCalc();
	void SkylabSaturnIBLaunchUplink();
	void TransferTIToMPT();
	void TransferSPQToMPT();
	void TransferDKIToMPT();
	void TransferDescentPlanToMPT();
	void TransferPoweredDescentToMPT();
	void TransferPoweredAscentToMPT();
	void TransferGPMToMPT();
	void MPTDirectInputCalc();
	void MPTTLIDirectInput();
	void AbortScanTableCalc();
	void TransferLOIorMCCtoMPT();
	void TransferRTEToMPT();
	void SLVNavigationUpdateCalc();
	void SLVNavigationUpdateUplink();
	void UpdateGRRTime();
	bool vesselinLOS();
	void MinorCycle(double SimT, double SimDT, double mjd);

	void UplinkData(bool isCSM);
	void UplinkData2(bool isCSM);
	void UplinkDataV70V73(bool v70, bool isCSM);
	void send_agc_key(char key, bool isCSM);
	void uplink_word(char *data, bool isCSM);
	void P30UplinkCalc(bool isCSM);
	void P30Uplink(bool isCSM);
	void RetrofireEXDVUplinkCalc(char source, char column);
	void RetrofireEXDVUplink();
	void EntryUplinkCalc();
	void EntryUpdateUplink(void);
	void REFSMMATUplink(bool isCSM);
	void StateVectorUplink(int type);
	void TLANDUplinkCalc(void);
	void TLANDUplink(void);
	void AGCClockIncrementUplink(bool csm);
	void AGCLiftoffTimeIncrementUplink(bool csm);
	void EMPP99Uplink(int i);
	void ManeuverPAD();
	void EntryPAD();
	void TPIPAD();
	void TLI_PAD();
	void PDI_PAD();
	void MapUpdate();
	void NavCheckPAD();
	void AP11AbortCoefUplink();
	void AP12AbortCoefUplink();
	void DetermineGMPCode();
	void NodeConvCalc();
	void SendNodeToSFP();
	void CalculateTPITime();
	void GetStateVectorFromAGC(bool csm);
	void GetStateVectorFromIU();
	void GetStateVectorsFromAGS();
	void VectorCompareDisplayCalc();
	void UpdateTLITargetTable();
	void GenerateSpaceDigitalsNoMPT();
	void LUNTARCalc();
	int GetVesselParameters(int Thruster, int &Config, int &TVC, double &CSMMass, double &LMMass);

	int startSubthread(int fcn);
	int subThread();

	//EPHEM PROGRAM
	void GenerateAGCEphemeris();
	int agcCelBody_RH(CELBODY *Cel, double mjd, int Flags, VECTOR3 *Pos = NULL, VECTOR3 *Vel = NULL);
	int agcCelBody_LH(CELBODY *Cel, double mjd, int Flags, VECTOR3 *Pos = NULL, VECTOR3 *Vel = NULL);
	void AGCEphemeris(double T0, int Epoch, double TEphem0);
	void AGCCorrectionVectors(double mjd_launchday, double dt_UNITW, double dt_504LM, int mission, bool isCMC);
	void GenerateAGCCorrectionVectors();

	// SUBTHREAD MANAGEMENT
	HANDLE hThread;
	int subThreadMode;										// What should the subthread do?
	int subThreadStatus;									// 0 = done/not busy, 1 = busy, negative = done with error

	ApolloRTCCMFDData g_Data;

	//TARGETING VESSELS
	VESSEL* vessel;
	VESSEL* target;
	int targetnumber;		//Vessel index for target

	//GENERAL PARAMETERS
	double P30TIG;				//Maneuver GET
	VECTOR3 dV_LVLH;			//LVLH maneuver vector
	int vesseltype;				// 0 = CSM, 1 = LM, 2 = MCC
	bool vesselisdocked;		// false = undocked, true = docked
	bool lemdescentstage;		//0 = ascent stage, 1 = descent stage
	bool inhibUplLOS;
	bool PADSolGood;
	int manpadenginetype;
	double t_TPI;				// Generally used TPI time

	//DOCKING INITIATION
	int TPI_Mode;
	double dt_TPI_sunrise;
	double t_TPIguess;

	//CONCENTRIC RENDEZVOUS PAGE
	int SPQMode;	//0 = CSI on time, 1 = CDH, 2 = optimum CSI
	double CSItime;	//Time of the CSI maneuver
	double CDHtime;	//Time of the CDH maneuver
	double SPQTIG;	//Time of ignition for concentric rendezvous maneuver
	int CDHtimemode; //CSI: 0 = fixed TIG at TPI, 1 = fixed DH at CDH. CDH: 0=Fixed, 1 = Find GETI
	VECTOR3 SPQDeltaV;

	//ORBIT ADJUSTMENT PAGE
	int GMPManeuverCode; //Maneuver code
	bool OrbAdjAltRef;	//0 = use mean radius, 1 = use launchpad or landing site radius
	double GMPApogeeHeight;		//Desired apoapsis height
	double GMPPerigeeHeight;	//Desired periapsis height
	double GMPWedgeAngle;
	double GMPManeuverHeight;
	double GMPManeuverLongitude;
	double GMPHeightChange;
	double GMPNodeShiftAngle;
	double GMPDeltaVInput;
	double GMPPitch;
	double GMPYaw;
	double GMPApseLineRotAngle;
	int GMPRevs;
	double SPSGET;		//Maneuver GET
	//0 = Apogee
	//1 = Equatorial crossing
	//2 = Perigee
	//3 = Longitude
	//4 = Height
	//5 = Time
	//6 = Optimum
	int GMPManeuverPoint;
	//0 = Plane Change
	//1 = Circularization
	//2 = Height Change
	//3 = Node Shift
	//4 = Apogee and perigee change
	//5 = Input maneuver
	//6 = Combination apogee/perigee change and node shift
	//7 = Shift line-of-apsides
	//8 = Combination height maneuver and plane change
	//9 = Combination circularization and plane change
	//10 = Combination circularization and node shift
	//11 = Combination height maneuver and node shift
	//12 = Combination apogee/perigee change and line-of-apsides shift
	int GMPManeuverType;

	//REFSMMAT PAGE
	double REFSMMATTime;
	int REFSMMATopt; //Displayed REFSMMAT page: 0 = P30 Maneuver, 1 = P30 Retro, 2 = LVLH, 3 = Lunar Entry, 4 = Launch, 5 = Landing Site, 6 = PTC, 7 = Attitude, 8 = LS during TLC
	int REFSMMATcur; //Currently saved REFSMMAT
	bool REFSMMATHeadsUp;

	//ENTY PAGE	
	double EntryTIGcor;
	double EntryLatcor;
	double EntryLngcor;
	VECTOR3 Entry_DV;
	double entryrange;
	double EntryRET05G; //Time of 0.05g
	double EntryRRT; //Time of entry interface (400k feet altitude)
	int landingzone; //0 = Mid Pacific, 1 = East Pacific, 2 = Atlantic Ocean, 3 = Indian Ocean, 4 = West Pacific
	int entryprecision; //0 = conic, 1 = precision, 2 = PeA=-30 solution
	double RTEReentryTime; //Desired landing time
	int RTECalcMode; // 0 = ATP Tradeoff, 1 = ATP Search, 2 = ATP Discrete, 3 = UA Search, 4 = UA Discrete
	int RTETradeoffMode; //0 = Near-Earth (F70), 1 = Remote-Earth (F71)
	int RTEASTType; //75 = unspecified, 76 = specific site, 77 = lunar search

	//STATE VECTOR PAGE
	bool SVSlot; //true = CSM, false = LEM
	double SVDesiredGET;
	VESSEL* svtarget;
	int svtargetnumber;

	//AGS STATE VECTOR
	double AGSEpochTime;
	VECTOR3 AGSPositionVector, AGSVelocityVector;
	AP11AGSSVPAD agssvpad;

	//MANEUVER PAD PAGE
	AP11MNV manpad;
	AP11LMMNV lmmanpad;
	char GDCset[64];
	bool HeadsUp;
	VECTOR3 TPIPAD_dV_LOS, TPIPAD_BT;
	double TPIPAD_dH, TPIPAD_R, TPIPAD_Rdot, TPIPAD_ELmin5, TPIPAD_AZ, TPIPAD_ddH;
	int manpadopt; //0 = Maneuver PAD, 1 = TPI PAD, 2 = TLI PAD
	double sxtstardtime;
	double manpad_ullage_dt;
	bool manpad_ullage_opt; //true = 4 jets, false = 2 jets
	TLIPAD tlipad;
	AP11PDIPAD pdipad;

	///ENTRY PAD PAGE
	AP11ENT lunarentrypad;
	AP7ENT earthentrypad;
	int entrypadopt; //0 = Earth Entry Update, 1 = Lunar Entry

	//MAP UPDATE PAGE
	AP10MAPUPDATE mapupdate;
	double GSAOSGET, GSLOSGET;
	int mappage, mapgs;
	double mapUpdateGET;

	//TLI PAGE
	//0 = TLI (nodal), 1 = TLI (free return)
	int TLImaneuver;

	//TLCC PAGE
	VECTOR3 R_TLI, V_TLI;
	int TLCCSolGood;

	//LANDMARK TRACKING PAGE
	AP11LMARKTRKPAD landmarkpad;
	double LmkLat, LmkLng;
	double LmkTime;

	//VECPOINT PAGE
	int VECoption;		//0 = Point SC at body, 1 = Open hatch thermal control, 2 = Point AOT with CSM
	int VECdirection;	//0 = +X, 1 = -X, 2 = +Y,3 = -Y,4 = +Z, 5 = -Z
	OBJHANDLE VECbody;	//handle for the desired body
	VECTOR3 VECangles;	//IMU angles

	//DOI Page
	VECTOR3 DOI_dV_LVLH;				//Integrated DV Vector

	//Skylab Page
	int Skylabmaneuver;					//0 = Presettings, 1 = NC1, 2 = NC2, 3 = NCC, 4 = NSR, 5 = TPI, 6 = TPM, 7 = NPC
	bool Skylab_NPCOption;				//0 = NC1 or NC2 with out-of-plane component, setting up a NPC maneuver 90� later
	bool Skylab_PCManeuver;				//0 = NC1 is setting up NPC, 1 = NC2 is setting up NPC
	double SkylabTPIGuess;
	double Skylab_n_C;
	double SkylabDH1;					//Delta Height at NCC
	double SkylabDH2;					//Delta Height at NSR
	double Skylab_E_L;
	bool SkylabSolGood;
	VECTOR3 Skylab_dV_NSR, Skylab_dV_NCC;//, Skylab_dV_NPC;
	double Skylab_dH_NC2, Skylab_dv_NC2, Skylab_dv_NCC;
	double Skylab_t_NC1, Skylab_t_NC2, Skylab_t_NCC, Skylab_t_NSR, Skylab_dt_TPM; //Skylab_t_NPC

	//Terrain Model
	double TMLat, TMLng, TMAzi, TMDistance, TMStepSize, TMAlt;

	//LM Ascent PAD
	AP11LMASCPAD lmascentpad;
	double t_LunarLiftoff;

	//Powered Descent Abort Program
	int PDAPEngine;	//0 = DPS/APS, 1 = APS
	bool PDAPTwoSegment;	//false = One Segment (Luminary099, FP6), true = Two Segment (Luminary116 and later, FP7 and later)
	double PDAPABTCOF[8];	//Luminary099 abort coefficients
	double DEDA224, DEDA225, DEDA226;
	int DEDA227;
	double PDAP_J1, PDAP_K1, PDAP_J2, PDAP_K2, PDAP_Theta_LIM, PDAP_R_amin;

	//Erasable Memory Programs
	int EMPUplinkType;	// 0 = P99
	int EMPUplinkNumber;

	//NAV CHECK PAGE
	AP7NAV navcheckpad;

	//DAP PAD PAGE
	AP10DAPDATA DAP_PAD;

	//LVDC PAGE
	double LVDCLaunchAzimuth;

	//AGC EPHEMERIS
	int AGCEphemOption;	//0 = AGC ephemerides, 1 = AGC precession/nutation/libration correction vectors
	int AGCEphemBRCSEpoch;
	double AGCEphemTEphemZero;
	double AGCEphemTIMEM0;
	double AGCEphemTEPHEM;
	double AGCEphemTLAND;
	int AGCEphemMission;
	bool AGCEphemIsCMC;

	//NODAL TARGET CONVERSION
	bool NodeConvOpt; //false = EMP to selenographc, true = selenographic to EMP
	double NodeConvLat;
	double NodeConvLng;
	double NodeConvGET;
	double NodeConvHeight;
	double NodeConvResLat;
	double NodeConvResLng;

	//SPACE DIGITALS
	int SpaceDigitalsOption;
	double SpaceDigitalsGET;

	//UPLINK
	double AGCClockTime[2];
	double RTCCClockTime[2];
	double DeltaClockTime[2];
	double DesiredRTCCLiftoffTime[2];
	int iuUplinkResult; //0 = no uplink, 1 = uplink accepted, 2 = vessel has no IU, 3 = uplink rejected, 4 = No targeting parameters

	//LUNAR TARGETING PROGRAM
	double LUNTAR_lat;
	double LUNTAR_lng;
	double LUNTAR_bt_guess;
	double LUNTAR_pitch_guess;
	double LUNTAR_yaw_guess;
	double LUNTAR_TIG;
	LunarTargetingProgramOutput LUNTAR_Output;

private:

	AR_GCore* GC;
};




#endif // !__ARCORE_H