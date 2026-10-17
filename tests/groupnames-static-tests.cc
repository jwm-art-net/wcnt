    const char* m = "clock1";
    const char* g = "group1";
    const char* v = "voice1";
    const char* c = "chain1";

    char* u1 = groupnames::str_make_username(m, groupnames::GROUP, g);
    std::cout << m << " >> " << g << ": '" << u1 << "'" << std::endl;

    char* u2 = groupnames::str_make_username(u1, groupnames::VOICE, v);
    std::cout << u1 << " >> " << v << ": '" << u2 << "'" << std::endl;

    char* u3 = groupnames::str_make_username(u2, groupnames::CHAIN, c);
    std::cout << u2 << " >> " << c <<": '" << u3 << "'" << std::endl;

    char* gm = 0;
    char* gg = 0;
    char* gv = 0;
    char* gc = 0;

    gm = groupnames::str_get_name(groupnames::MODULE, u3);
    gg = groupnames::str_get_name(groupnames::GROUP,  u3);
    gv = groupnames::str_get_name(groupnames::VOICE,  u3);
    gc = groupnames::str_get_name(groupnames::CHAIN,  u3);

    std::cout << "module: '" << (gm ? gm : "0x0") << "' group: '" << (gg ? gg : "0x0") << "'";
    std::cout << " voice: '" << (gv ? gv : "0x0") << "' chain: '" << (gc ? gc : "0x0") << "'" << std::endl;
    std::cout << "done..." << std::endl;

    if (groupnames::str_has_name(groupnames::VOICE, u3))
        std::cout << "has voice\n";
    else
        std::cout << "no voice\n";

    if (groupnames::str_has_name(groupnames::GROUP, u3))
        std::cout << "has group\n";
    else
        std::cout << "no group\n";

    if (groupnames::str_has_name(groupnames::CHAIN, u3))
        std::cout << "has chain\n";
    else
        std::cout << "no chain\n";

    if (groupnames::str_has_name(groupnames::MODULE, u3))
        std::cout << "has module\n";
    else
        std::cout << "no module\n";

    std::cout << std::endl;

    delete [] u2;

    u2 = groupnames::str_make_username(u3, groupnames::GROUP, "lfo1");

    std::cout << u3 << " >> " << "lfo1" <<": '" << u2 << "'" << std::endl;

    delete [] u3;

    u3 = groupnames::str_make_username(u2, groupnames::MODULE, "wave2");

    std::cout << u2 << " >> " << "wave2" <<": '" << u3 << "'" << std::endl;

    delete [] u2;

    u2 = groupnames::str_make_username(u3, groupnames::VOICE, "bass-x");

    std::cout << u3 << " >> " << "bass-x" <<": '" << u2 << "'" << std::endl;

    delete [] u3;

    u3 = groupnames::str_make_username(u2, groupnames::CHAIN, "filter");

    std::cout << u2 << " >> " << "filter" <<": '" << u3 << "'" << std::endl;

    delete [] gm;
    delete [] gg;
    delete [] gv;
    delete [] gc;
    delete [] u3;
    delete [] u2;
    delete [] u1;

    return 0;
