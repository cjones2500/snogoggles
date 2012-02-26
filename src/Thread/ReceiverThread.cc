#include <avalanche.hpp>

#include <RAT/DS/EV.hh>
#include <RAT/DS/PMTProperties.hh>
#include <RAT/DS/Run.hh>
#include <RAT/Pack.hh>
using namespace RAT;

#include <TTree.h>
#include <TFile.h>
using namespace ROOT;

#include <sstream>
#include <iostream>
using namespace std;

#include <Viewer/ReceiverThread.hh>
#include <Viewer/EventData.hh>
#include <Viewer/Semaphore.hh>
using namespace Viewer;

void
ReceiverThread::Run()
{
  // Must load a DS run for the PMT Positions (replace this with db access when possible...)
  EventData& events = EventData::GetInstance();
  if( fNumReceivedEvents == 0 )
    {
      LoadRootFile();
      events.SetRun( fRun );
      fFile->Close();
      delete fFile;
      delete fDS;
      delete fRun;
    }

  // create a client, listening for objects
  avalanche::client client;
  client.addDispatcher( fPort.c_str() );
  // receive RAT::DS::PackedRec objects, blocks until recieves packet
  cout << "Listening" << endl;
  //RAT::DS::PackedRec* rec = (RAT::DS::PackedRec*) client.recvObject(RAT::DS::PackedRec::Class());
  //if( rec && rec->RecordType == 1 ) //Detector event (why not enum?)
  
  // Temp fix for builder strangeness
  RAT::DS::PackedEvent* event = (RAT::DS::PackedEvent*) client.recv();
  if( event )
    {
      cout << "Got an event" << endl;
      //RAT::DS::PackedEvent* event = dynamic_cast<RAT::DS::PackedEvent*> (rec->Rec);
      RAT::DS::Root* rDS = RAT::Pack::UnpackEvent( event, NULL, NULL );
      events.AddEV( rDS->GetEV(0), fNumReceivedEvents );
      fNumReceivedEvents++;
      if( fNumReceivedEvents == 1 )
       fSemaphore.Signal();
      delete rDS;
      delete event;
    }
 
}

void
ReceiverThread::LoadRootFile()
{
  fFile = new TFile( "Temp.root", "READ" );
 
  fTree = (TTree*)fFile->Get( "T" );
  fDS = new RAT::DS::Root();
  fTree->SetBranchAddress( "ds", &fDS );

  fRunTree = (TTree*)fFile->Get( "runT" );
  fRun = new RAT::DS::Run();
  fRunTree->SetBranchAddress( "run", &fRun );
  fRunTree->GetEntry();
}