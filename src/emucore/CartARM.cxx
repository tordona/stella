//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2021 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "System.hxx"
#include "Settings.hxx"
#include "CartARM.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeARM::CartridgeARM(const string& md5, const Settings& settings)
  : Cartridge(settings, md5)
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeARM::setInitialState()
{
  bool devSettings = mySettings.getBool("dev.settings");

  if(devSettings)
  {
    myIncCycles = mySettings.getBool("dev.thumb.inccycles");
    myThumbEmulator->setChipType(static_cast<Thumbulator::ChipType>(mySettings.getInt("dev.thumb.chiptype")));
    myThumbEmulator->setMamMode(static_cast<Thumbulator::MamModeType>(mySettings.getInt("dev.thumb.mammode")));
  }
  else
  {
    myIncCycles = false;
  }
  enableCycleCount(devSettings);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeARM::updateCycles(int cycles)
{
  if(myIncCycles)
    mySystem->incrementCycles(cycles);
#ifdef DEBUGGER_SUPPORT
  myPrevStats = myStats;
  myStats = myThumbEmulator->stats();
  myPrevCycles = myCycles;
  myCycles = myThumbEmulator->cycles();
#endif
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeARM::incCycles(bool enable)
{
  myIncCycles = enable;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeARM::cycleFactor(double factor)
{
  myThumbEmulator->cycleFactor(factor);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeARM::save(Serializer& out) const
{
#ifdef DEBUGGER_SUPPORT
  try
  {
    out.putInt(myPrevCycles);
    out.putInt(myPrevStats.instructions);
    out.putInt(myCycles);
    out.putInt(myStats.instructions);
  }
  catch(...)
  {
    cerr << "ERROR: CartridgeARM::save" << endl;
    return false;
  }
#endif
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeARM::load(Serializer& in)
{
#ifdef DEBUGGER_SUPPORT
  try
  {
    myPrevCycles = in.getInt();
    myPrevStats.instructions = in.getInt();
    myCycles = in.getInt();
    myStats.instructions = in.getInt();
  }
  catch(...)
  {
    cerr << "ERROR: CartridgeARM::load" << endl;
    return false;
  }
#endif
  return true;
}