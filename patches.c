
/*From: Soenke Lambert <s.lambert@mittwald.de> */
static void togglefullscr(const Arg *arg);



/*From: Soenke Lambert <s.lambert@mittwald.de> */
void
togglefullscr(const Arg *arg)
{
    if(selmon->sel){
        setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
    }
}
