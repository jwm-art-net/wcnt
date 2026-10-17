#include "../include/groupnames.h"

#include "../include/debug.h"
#include "../include/globals.h"

#include <cstring>

groupnames::groupnames():
  components(0, 0, 0, 0, 0)
{
}

groupnames::groupnames(COMPONENT c, const char* str):
  components(0, 0, 0, 0, 0)
{
    if (str && strlen(str) <= wcnt::max_name_len)
        set_name(c, str);
}

groupnames::~groupnames()
{
    for (int i = 0; i < 5; i++)
        if (components[i])
            delete [] components[i];
}

char groupnames::seperators[MODULE + 1] = { '\0', ':', '.', '~', '\0' };


const char* groupnames::set_name(COMPONENT c, const char* str)
{
    if (str && strlen(str) > wcnt::max_name_len)
        return 0;
    if (c != MODULE && !components[MODULE])
        return 0;

    // FIXME: put this in debug code later...
    for (int i = 1; str && i < 4; i++) {
        if (strchr(str, seperators[i])) {
            std::cout << "groupnames::set_name parameter error "
                    "contains restricted character '" << seperators[i]
                    << "'" << std::endl;
        }
    }
    const char* ptrs[MODULE + 1];

    for (int i = 0; i < 5; i++)
        ptrs[i] = components[i];
    ptrs[c] = str;

    char* u = str_make_username(ptrs[1], ptrs[2], ptrs[3], ptrs[4]);

    if (components[0])
        delete [] components[0];

    components[0] = u;

    if (components[c])
        delete [] components[c];

    if (str) {
        components[c] = new char[strlen(str) + 1];
        strcpy(components[c], str);
    }
    else
        components[c] = 0;

    return components[0];
}

const char* groupnames::set_username(const char* str)
{
    if (!str)
        return 0;
    const char* ps[] = { str, 0, 0, 0, 0 };
    for (int c = 1; c <= MODULE; c++) {
        ps[c] = strchr(str, seperators[c]);
        if (ps[c]) {
            int d;
            // don't go to zero, as no seperator...
            for (d = c - 1; d > 0; d--)
                if (ps[d]++) // ... to increment past
                    break;
            size_t l = ps[c] - ps[d];
            char* r = new char[l + 1];
            strncpy(r, ps[d], l);
            r[l] = '\0';
            components[c] = r;
        }
    }

    if (components[0])
        delete [] components[0];

    components[0] = str_make_username(components[1], components[2],
                                      components[3], components[4]);

    return components[0];
}


bool groupnames::excessive_username()
{
    return (components[0] && strlen(components[0]) > wcnt::max_username_len);
}


// does not set username, just makes one and returns it.
char* groupnames::make_username(COMPONENT c, const char* str) const
{
    if (str && strlen(str) > wcnt::max_name_len)
        return 0;
    const char* ptrs[MODULE + 1];
    for (int i = 0; i < MODULE + 1; i++)
        ptrs[i] = components[i];
    ptrs[c] = str;

    return str_make_username(ptrs[1], ptrs[2], ptrs[3], ptrs[4]);
}


char* groupnames::str_make_username(const char* str, COMPONENT comp, const char* compstr)
{
    if (!str)
        return 0;
    if (compstr && strlen(compstr) > wcnt::max_name_len)
        return 0;
    char* comps[] = { 0, 0, 0, 0, 0 };
    const char* pc = str;
    const char* pcend = pc + strlen(pc);
    for (int i = 1; i < 5; i++) {
        int s = groupnames::seperators[i];
        const char* ps = strchr(pc, s);
        size_t l = (ps ? (ps - pc) : 0);
        if (s == groupnames::seperators[comp]) {
            if (compstr) {
                comps[i] = new char[strlen(compstr) + 1];
                strcpy(comps[i], compstr);
            }
            else
                comps[i] = 0;
            if (l)
                pc += l + 1;
        }
        else {
            if (ps) {
                comps[i] = new char[l + 1];
                strncpy(comps[i], pc, l);
                comps[i][l] = '\0';
                pc += l + 1;
            }
        }
        if (pc >= pcend)
            break;
    }
    char* r = str_make_username(comps[1], comps[2], comps[3], comps[4]);
    for (int i = 1; i < 5; i++)
        delete [] comps[i];
    return r;
}

char* groupnames::str_make_username(const char* vs, const char* gs, const char* cs, const char* ms)
{
    size_t len = 0;

    const char* args[] = { 0, vs, gs, cs, ms };
    size_t lens[] = { 0, 0, 0, 0, 0 };

    for (int i = 1; i < 5; i++) {
        if (args[i]) {
            lens[i] = strlen(args[i]);
            if (lens[i] > wcnt::max_name_len)
                return 0;
            len += lens[i];
            if (seperators[i])
                len++;
        }
    }

    if (!len || len > wcnt::max_username_len)
        return 0;

    char* u = new char[len + 1];

    len = 0;
    for (int i = 1; i < 5; i++) {
        if (args[i]) {
            strcpy(&u[len], args[i]);
            len += lens[i];
            if (seperators[i])
                u[len++] = seperators[i];
        }
    }

    return u;
}

char* groupnames::str_get_name(COMPONENT comp, const char* str)
{
    if (!str)
        return 0;
    const char* ps[] = { str, 0, 0, 0, 0 };
    for (int c = 1; c <= MODULE; c++) {
        ps[c] = strchr(str, seperators[c]);
        if (c == comp) {
            if (ps[c]) {
                int d;
                // don't go to zero, as no seperator...
                for (d = c - 1; d > 0; d--)
                    if (ps[d]++) // ... to increment past
                        break;
                size_t l = ps[c] - ps[d];
                char* r = new char[l + 1];
                strncpy(r, ps[d], l);
                r[l] = '\0';
                return r;
            }
            return 0;
        }
    }
    return 0;
}

bool  groupnames::str_has_name(COMPONENT c, const char* str)
{
    if (str)
        return strchr(str, seperators[c]) != 0;
    return 0;
}

int groupnames::str_has_reserved_chr(const char* str)
{
    for (int i = 1; i < 5; i++)
        if (strchr(str, seperators[i]))
            return seperators[i];
    return 0;
}


