#ifndef EMBER_STYLE_H
#define EMBER_STYLE_H

typedef struct style
{
    ImVec4_c text_primary;
    ImVec4_c background_primary;
    ImVec4_c background_secondary; 
    ImVec4_c background_tertiary;
    ImVec4_c highlight_primary;
    ImVec4_c highlight_secondary;
} style;

void style_set(style* style);

#endif