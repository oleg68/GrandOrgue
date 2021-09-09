#include <iostream>

#include "GOSoundThread.h"
#include "GOSoundWorkItem.h"

void GOSoundWorkItem::CheckPoint(GOSoundThread * pThread, const char* ckpt)
{
  if (pThread)
  {
    pThread->SetLastCheckpoint(ckpt);
    if (pThread->ShouldStop())
      std::cout << ckpt << std::endl;
  }
}

