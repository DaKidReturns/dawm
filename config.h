/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 3;       /* snap pixel */
static const unsigned int gappx     = 0;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = {"monospace:size=10","Nerd Font:size=11","Font Awesome 5:size=9" };
//static const char *fonts[]          = {"Ubuntu:style=Light Italic:size=11","Symbols Nerd Font:style=2048-em:size=13" };
static const char *fonts[]          = {"Sarasa Mono K:size=11","Symbols Nerd Font:style=2048-em:size=13" };
//static const char *fonts[]          = {"ProggySquare:size=9","Font Awesome 5:size=9" };
static const int focusonwheel       = 1;
static const char dmenufont[]       = "Sarasa Mono K:size=12";
static const char normbg[]          = "#2e3440";
static const char normborder[]      = "#2e3440";
static const char normfg[]          = "#d8dee9";
static const char selfg[]           = "#eeeeee";
static const char selbg[]           = "#4c566a";
static const char selborder[]       = "#5b73a3";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfg, normbg, normborder },
	[SchemeSel]  = { selfg , selbg , selborder  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                     instance    title       tags mask     isfloating   monitor */
	{ "Gimp",                      NULL,       NULL,       0,            1,           -1 },
	{ "feh",                       NULL,       NULL,       0,            1,           -1 },
	{ "vlc",                       NULL,       NULL,       0,            1,           -1 },
    { "Pavucontrol",               NULL,       NULL,       0,            1,           -1 },
    { "Tor Browser",               NULL,       NULL,       1 << 8,       1,           -1 },
	{ "firefox",                   NULL,       NULL,       1 << 1,       0,           -1 },
    { "Thunar",                    NULL,       NULL,       1 << 2,       0,           -1 },
    { "Zathura",                   NULL,       NULL,       1 << 3,       0,           -1 },
    { "libreoffice-writer",        NULL,       NULL,       1 << 5,       0,           -1 },
    { "libreoffice-startcenter",   NULL,       NULL,       1 << 5,       0,           -1 },
    { "TelegramDesktop",           NULL,       NULL,       1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "<><",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[]         =     { "rofi", "-show", "run", NULL };
static const char *dmenucmd[]         =     { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbg , "-nf", normfg, "-sb", selbg,  "-sf", selfg, NULL };
static const char *termcmd[]          =     { "myterminal", NULL }; //It is a custom bash command that launches your terminal with extra arguments any (so you dont need to recompile this stuff again)
static const char *volinccmd[]        =     { "volchange","inc","3",NULL };
static const char *voldeccmd[]        =     { "volchange","dec","3",NULL };
static const char *brightnessup[]     =     { "xbacklight", "-inc","10" ,NULL };
static const char *brightnessdown[]   =     { "xbacklight", "-dec","10" ,NULL };
static const char *screenshotcmd[]    =     { "scrot","SS_%Y%m%d%H%M%S_$wx$h.png","-q","30", "-e", "mv $f ~/Pictures/Screenshots/" ,NULL };
static const char *browser[]          =     { "firefox", "--no-remote", NULL };
static const char *firefoxProfile[]   =     { "firefox", "--no-remote","--ProfileManager", NULL };

//static const char *volinccmd[]        =     { "amixer","sset","PCM","playback","5+",NULL };
//static const char *voldeccmd[]        =     { "amixer","sset","PCM","playback","5-",NULL };
//static const char *volmutecmd[]       =     { "amixer", "sset","Master","playback","toggle", NULL };

static Key keys[] = {
	/* modifier                             key                        function        argument */
	{ MODKEY,                               XK_p,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,                               XK_u,                      spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,                     XK_b,                      togglebar,      {0} },
	{ MODKEY,                               XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,                               XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,                               XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,                               XK_d,                      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,                     XK_j,                      inplacerotate,  {.i = -2 } },
	{ MODKEY|ShiftMask,                     XK_k,                      inplacerotate,  {.i = +2 } },
	{ MODKEY|ShiftMask,                     XK_h,                      inplacerotate,  {.i = -1 } },
	{ MODKEY|ShiftMask,                     XK_l,                      inplacerotate,  {.i = +1 } },
	{ MODKEY,                               XK_h,                      setmfact,       {.f = -0.05 } },
	{ MODKEY,                               XK_l,                      setmfact,       {.f = +0.05 } },
	{ MODKEY,                               XK_Return,                 zoom,           {0} },
	{ MODKEY,                               XK_Tab,                    view,           {0} },
    { MODKEY|ShiftMask,                     XK_q,                      killclient,     {0} },
    { MODKEY,                               XK_t,                      setlayout,      {.v = &layouts[0] } },
	{ MODKEY,                               XK_f,                      setlayout,      {.v = &layouts[1] } },
	{ MODKEY,                               XK_m,                      setlayout,      {.v = &layouts[2] } },
	{ MODKEY,                               XK_space,                  setlayout,      {0} },
	{ MODKEY|ShiftMask,                     XK_space,                  togglefloating, {0} },
	{ MODKEY|ShiftMask,                     XK_f,                      togglefullscr,  {0} },
	{ MODKEY,                               XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,                     XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,                               XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                               XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,                     XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,                     XK_period,                 tagmon,         {.i = +1 } },
	{ MODKEY,                               XK_minus,                  setgaps,        {.i = -1 } },
	{ MODKEY,                               XK_equal,                  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,                     XK_equal,                  setgaps,        {.i = 0 } },
	{ MODKEY,                               XK_b,                      spawn,          {.v = browser } },
	{ MODKEY|ControlMask,                   XK_b,                      spawn,          {.v = firefoxProfile } },
	{ ControlMask|Mod1Mask,                 XK_l,                      spawn,          SHCMD("lock") },
	{ MODKEY|ShiftMask,/*Normie Nightmare*/ XK_n,                      spawn,          SHCMD("xinput disable 'DELL083F:00 04F3:3088 Touchpad'") },
    { MODKEY,                               XK_r,                      spawn,          SHCMD("xset r rate 250 25") },
	{ 0,                                    0x1008ff13,                spawn,          {.v = volinccmd} },
	{ 0,                                    0x1008ff11,                spawn,          {.v = voldeccmd} },
	{ 0,                                    0x1008ff12,                spawn,          {.v = (const char*[]){"volchange","toggle",NULL} } },
	{ 0,                                    0x1008ff02,                spawn,          {.v = brightnessup } },
	{ 0,                                    0x1008ff03,                spawn,          {.v = brightnessdown} },
	{ 0,      /*Play Button    */           0x1008ff14,                spawn,          {.v = (const char*[]){"mpc","toggle",NULL}} },
	{ 0,      /*Forward button */           0x1008ff17,                spawn,          {.v = (const char*[]){"mpc","next",NULL}} },
	{ 0,      /*Rewind Button  */           0x1008ff16,                spawn,          {.v = (const char*[]){"mpc","prev",NULL}} },
    { 0,                                    0xff61,                    spawn,          {.v = screenshotcmd } },
	TAGKEYS(                                XK_1,                      0)
	TAGKEYS(                                XK_2,                      1)
	TAGKEYS(                                XK_3,                      2)
	TAGKEYS(                                XK_4,                      3)
	TAGKEYS(                                XK_5,                      4)
	TAGKEYS(                                XK_6,                      5)
	TAGKEYS(                                XK_7,                      6)
	TAGKEYS(                                XK_8,                      7)
	TAGKEYS(                                XK_9,                      8)
	{ MODKEY|ShiftMask,                     XK_Escape,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

