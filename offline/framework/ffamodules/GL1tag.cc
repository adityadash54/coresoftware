#include "GL1tag.h"

#include <ffaobjects/SyncDefs.h>
#include <ffaobjects/SyncObject.h>
#include <ffaobjects/SyncObjectv1.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>  // for SubsysReco

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>          // for PHNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE
#include <phool/recoConsts.h>

#include <iostream>

GL1tag::GL1tag(const std::string &name)
  : SubsysReco(name)
{
  return;
}

int GL1tag::Init(PHCompositeNode *topNode)
{
  int iret = CreateNodeTree(topNode);
  return iret;
}

int GL1tag::InitRun(PHCompositeNode *topNode)
{
  SyncObject *syncobject = findNode::getClass<SyncObject>(topNode, syncdefs::SYNCNODENAME);
  recoConsts *rc = recoConsts::instance();
  syncobject->RunNumber(rc->get_IntFlag("RUNNUMBER"));
  return Fun4AllReturnCodes::EVENT_OK;
}

int GL1tag::CreateNodeTree(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode;
  dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
  {
    std::cout << PHWHERE << " DST Node is missing doing nothing" << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  SyncObject *syncobject = findNode::getClass<SyncObject>(topNode, syncdefs::SYNCNODENAME);
  if (!syncobject)
  {
    syncobject = new SyncObjectv1();
    PHIODataNode<PHObject> *SyncObjectNode = new PHIODataNode<PHObject>(syncobject, syncdefs::SYNCNODENAME, "PHObject");  // contains PHObject
    dstNode->addNode(SyncObjectNode);
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int GL1tag::process_event(PHCompositeNode *topNode)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  SyncObject *syncobject = findNode::getClass<SyncObject>(topNode, syncdefs::SYNCNODENAME);
  if (!syncobject)
  {
    std::cout << PHWHERE << " No Synchronisation Object, no parallel reading of multiple inputs" << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  
  if(!first_event){
    syncobject->EventCounter(prevevent_EventCounter);
    syncobject->EventNumber(prevevent_EventCounter);
    syncobject->RunNumber(prevevent_RunNumber);
      
    if (forced_segment >= 0)
    {
        syncobject->SegmentNumber(forced_segment);
    }
    else
    {
        syncobject->SegmentNumber(prevevent_SegmentNumber);
    }
  }
    
  prevevent_EventCounter=se->EventCounter();
  prevevent_EventNumber=se->EventNumber();
  prevevent_RunNumber=se->RunNumber();
  prevevent_SegmentNumber=se->SegmentNumber();
  
  if(first_event){
    first_event=false;
    return Fun4AllReturnCodes::ABORTEVENT;
  }
    
  if (Verbosity() > 0)
  {
    syncobject->identify();
  }
  return Fun4AllReturnCodes::EVENT_OK;
}
