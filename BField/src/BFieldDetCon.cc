#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

#include "BFieldDetCon.hh"

BFieldDetCon::BFieldDetCon(G4VPhysicalVolume* setWorld = 0)
{
//	ConstructMaterials();
	DefineDimensions();
	world = setWorld;
}

BFieldDetCon::~BFieldDetCon()
{
//	DestructMaterials();
}

void BFieldDetCon::DefineDimensions()
{
	// World dimensions (Laboratory size)
//	labX = 5000.0 * mm; // World x dimension
//	labY = 5000.0 * mm; // World y dimension
//	labZ = 5000.0 * mm; // World z dimension

	// Magnetic field region dimensions
	iR   =   10.0 * mm; // Inner radius for magnetic field region
	oR   =  300.0 * mm; // Outer radius for magnetic field region
	hZ   =  900.0 * mm; // Length of magnetic field region
}

G4VPhysicalVolume* BFieldDetCon::Construct()
{
/*
	// World
	G4Box* labSolid = new G4Box("labSolid", labX / 2, labY / 2, labZ / 2);
	G4LogicalVolume* labLV = new G4LogicalVolume(labSolid, Air, "labLV");
	G4VPhysicalVolume* labPV = new G4PVPlacement(0, G4ThreeVector(), "labPV", labLV, 0, false, 0);

	return labPV;
*/

	// Magnetic field region
	G4Material* dummyMat = new G4Material("dummyMat", 1.0, 1.0 * g/mole, 1e-25 * g/cm3);
	G4Tubs* magSolid = new G4Tubs("magSolid", iR, oR, 0.5*hZ, 0.0 * deg, 360.0 * deg);
	G4LogicalVolume* magLV = new G4LogicalVolume(magSolid, dummyMat, "magLV");
	new G4PVPlacement(0, G4ThreeVector(), "magPV", magLV, world, false, 0);

	// Magnetic field
	magField = new G4UniformMagField(G4ThreeVector(0.0, 0.0, 3.0 * tesla)); // Unit is tesla, maybe.
	G4FieldManager* fieldMgr = new G4FieldManager(magField);
	fieldMgr -> SetDetectorField(magField);
	fieldMgr -> CreateChordFinder(magField);
	world -> GetLogicalVolume() -> GetDaughter(0) -> GetLogicalVolume() -> SetFieldManager(fieldMgr, true);
//	magLV -> SetFieldManager(fieldMgr, true);

	return world;
}

void BFieldDetCon::ConstructMaterials()
{
	const G4double labTemp = 300.0 * kelvin; // Room temperature

	elN  = new G4Element("Nitrogen",  "N",  7, 14.00674*g/mole);
	elO  = new G4Element(  "Oxygen",  "O",  8, 15.9994 *g/mole);
	elAr = new G4Element(   "Argon", "Ar", 18, 39.948  *g/mole);

	Air  = new G4Material("Air", 1.2929e-10*g/cm3, 3, kStateGas, labTemp); // Almost vacuum
	Air -> AddElement(elN, 75.47/99.95);
	Air -> AddElement(elO, 23.20/99.95);
	Air -> AddElement(elAr, 1.28/99.95);
}

void BFieldDetCon::DestructMaterials()
{
	delete Air;

	delete elAr;
	delete elO;
	delete elN;
}
