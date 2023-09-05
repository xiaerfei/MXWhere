//
//  MXExternalSourceParse.cpp
//  MXWhere
//
//  Created by sharexia on 9/4/23.
//

#include "MXExternalSourceParse.hpp"
void sourceParseThread(int n) {
    
}

int MXExternalSourceParse::Start(char *path, int index) {
    m_filePath = path;
    m_index    = index;
    
    m_thread = std::thread(sourceParseThread, 0);
    
    return 0;
}

int MXExternalSourceParse::Stop() {
    
    return 0;
}

bool MXExternalSourceParse::Running() {
    return m_running;
}


