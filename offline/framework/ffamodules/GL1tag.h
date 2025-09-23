#ifndef FFAMODULES_GL1TAG_H
#define FFAMODULES_GL1TAG_H

#include <fun4all/SubsysReco.h>

#include <string>

class PHCompositeNode;

class GL1tag : public SubsysReco
{
 public:
  GL1tag(const std::string &name = "SYNC");
  ~GL1tag() override {}

  int Init(PHCompositeNode *topNode) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;

  void SegmentNumber(int i) { forced_segment = i; }

 private:
  static int CreateNodeTree(PHCompositeNode *topNode);
  // just if we need to override the segment for e.g. embedding
  // where we want to reuse hijing files which normally set
  // the segment number
  int forced_segment = -1;
    
  int prevevent_EventCounter=0;
  int prevevent_EventNumber=0;
  int prevevent_RunNumber=0;
  int prevevent_SegmentNumber=0;
    
  bool first_event=true;
    
};

#endif /* FFAMODULES_GL1TAG_H */
