#pragma once

#include "proc_list.h"

int ProcList::InsertProcName(ProcName proc_name) {
  if (proc_name_set_.insert(proc_name).second) {
    int index = num_procs;
    proc_name_list_.push_back(proc_name);
    proc_name_twin_list_.push_back(make_pair(proc_name, proc_name));
    proc_index_map_[proc_name] = index;
    index_proc_map_[index] = proc_name;
    num_procs++;
    return index;
  }
  return -1;
}

ProcNameList ProcList::GetAllProcName() { return proc_name_list_; }

bool ProcList::IsProcName(ProcName proc_name) {
  return proc_name_set_.find(proc_name) != proc_name_set_.end();
}

ProcNamePairList ProcList::GetAllProcNameTwin() { return proc_name_twin_list_; }

IndexProcMap ProcList::GetIndexToProcMapping() {
  return index_proc_map_;
}

ProcIndexMap ProcList::GetProcToIndexMapping() {
  return proc_index_map_;
}