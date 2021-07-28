/*
// From: Soenke Lambert <s.lambert@mittwald.de>
static void togglefullscr(const Arg *arg);

// From: From: Miles Alan <m@milesalan.com>
static void inplacerotate(const Arg *arg);

/*From:MLquest8 <miskuzius@gmail.com>
static void attachtop(Client *c);

  Maciej Janicki mail@macjanicki.eu
  David Julien swy7ch@protonmail.com (20200504-b2e1dfc port)
  Klein Bottle kleinbottle4@gmail.com (dwm-fullgaps-toggle...)

static void setgaps(const Arg *arg);
*/

/*From: Soenke Lambert <s.lambert@mittwald.de> */
void
togglefullscr(const Arg *arg)
{
    if(selmon->sel){
        setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
    }
}


/*From: From: Miles Alan <m@milesalan.com> */
void
insertclient(Client *item, Client *insertItem,int after)
{
    Client *c;
    if (item == NULL || insertItem == NULL || item == insertItem)
        return; // Dont do anything if 1. there is no item to insert 2. if there is only one item on the screen
    detach(insertItem);//Dereferecnces all the child client(ones that come after (O(n)))
    if(!after && selmon->clients == item){
        attach(insertItem);
        return;
    }
    if(after){
        c = item;
    } else {
        for (c = selmon->clients; c; c=c->next){
            if(c->next == item)
                break;
        }
    }
    insertItem->next = c->next;
    c->next = insertItem;
}

/*From: From: Miles Alan <m@milesalan.com> */
void
inplacerotate(const Arg *arg)
{
    if(!selmon->sel || (selmon->sel->isfloating && !arg->f))
        return;

    unsigned int selidx=0, i=0;
    Client *c =NULL, *stail=NULL, *mhead = NULL, *mtail = NULL, *shead=NULL;

    //Determine the position for inserting the client
    for (c = selmon->clients; c; c=c->next){
        if(ISVISIBLE(c)&& !c->isfloating){
            if(selmon->sel == c) { selidx=i; }
            if(i == selmon->nmaster - 1){ mtail = c; }
            if(i == selmon->nmaster){ shead = c; }
            if( mhead == NULL ) {mhead = c;}
            stail = c;
            i++;
        }
    }

    //All clients rotate
    if(arg->i == 2)  insertclient(selmon->clients, stail, 0);
    if(arg->i == -2) insertclient(stail, selmon->clients, 1);

    //Stack xor master
    if(arg->i == -1 && selidx >= selmon->nmaster) insertclient(stail,shead,1);
    if(arg->i ==  1 && selidx >= selmon->nmaster) insertclient(shead,stail,0);
    if(arg->i == -1 && selidx <  selmon->nmaster) insertclient(mtail,mhead,1);
    if(arg->i ==  1 && selidx <  selmon->nmaster) insertclient(mhead,mtail,0);

    //Restore focus position
    i = 0;
    for(c = selmon->clients; c; c=c->next){
        if (!ISVISIBLE(c) || c->isfloating)
            continue;
        if(selidx == i)
            break;
    }
    arrange(selmon);
    focus(c);
}

/*From:MLquest8 <miskuzius@gmail.com>*/
void
attachtop(Client *c)
{
    int n;
    Monitor *m = selmon;
    Client *below;

    for (n = 1, below = c->mon->clients;
            below && below->next && (below->isfloating || !ISVISIBLEONTAG(below,c->tags) || n != m->nmaster);
            n = below->isfloating || !ISVISIBLEONTAG(below,c->tags)? n + 0 : n + 1, below=below->next);
    c->next=NULL;
    if (below){
        c->next = below->next;
        below->next = c;
    }
    else
        c->mon->clients = c;
}

void
setgaps(const Arg *arg)
{
    if ((arg->i == 0 ) || (selmon->gappx + arg->i < 0))
        selmon->gappx = 0;
    else
        selmon->gappx += arg->i;
    arrange(selmon);
}



void
buttonpress(XEvent *e)
{
	unsigned int i, x, click;
	Arg arg = {0};
	Client *c;
	Monitor *m;
	XButtonPressedEvent *ev = &e->xbutton;

	click = ClkRootWin;
	/* focus monitor if necessary */
	if ((m = wintomon(ev->window)) && m != selmon) {
		unfocus(selmon->sel, 1);
		selmon = m;
		focus(NULL);
	}
	if (ev->window == selmon->barwin) {
		i = x = 0;
		do
			x += TEXTW(tags[i]);
		while (ev->x >= x && ++i < LENGTH(tags));
		if (i < LENGTH(tags)) {
			click = ClkTagBar;
			arg.ui = 1 << i;
		} else if (ev->x < x + blw)
			click = ClkLtSymbol;
		else if (ev->x > selmon->ww - (int)TEXTW(stext))
			click = ClkStatusText;
		else
			click = ClkWinTitle;
	} else if ((c = wintoclient(ev->window))) {
		focus(c);
		restack(selmon);
		XAllowEvents(dpy, ReplayPointer, CurrentTime);
		click = ClkClientWin;
	}
	for (i = 0; i < LENGTH(buttons); i++)
		if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
		&& CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
			buttons[i].func(click == ClkTagBar && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
}

Monitor *
createmon(void)
{
	Monitor *m;

	m = ecalloc(1, sizeof(Monitor));
	m->tagset[0] = m->tagset[1] = 1;
	m->mfact = mfact;
	m->nmaster = nmaster;
	m->showbar = showbar;
	m->topbar = topbar;
  m->gappx = gappx;
	m->lt[0] = &layouts[0];
	m->lt[1] = &layouts[1 % LENGTH(layouts)];
	strncpy(m->ltsymbol, layouts[0].symbol, sizeof m->ltsymbol);
	return m;
}

//Gapped Tiling
void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? m->ww * m->mfact : 0;
	else
		mw = m->ww - m->gappx;
	for (i = 0, my = ty = m->gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my ) / (MIN(n,m->nmaster)-i) - m->gappx;
			resize(c, m->wx + m->gappx, m->wy + my, mw - (2*c->bw) - m->gappx, h - (2*c->bw), 0);
			my += HEIGHT(c) + m->gappx;
		} else {
			h = (m->wh - ty ) / (n - i) - m->gappx;
			resize(c, m->wx + mw + m->gappx, m->wy + ty, m->ww - mw - (2*c->bw) - 2*m->gappx, h - (2*c->bw), 0);
			ty += HEIGHT(c) + m->gappx;
		}
}
