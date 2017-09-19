#include "TriangleAttacher.h"


bool TriangleAttacher::attach(Surface* cp, Triangle* pl, Checker* ch)
{
    // check Triangle is in near polygon or not
    if (!isNeighbor(cp, pl)) return false;

    // TODO : overlap check (projection)
    Vector_3 pl_nv = pl->getNormal();

    Vertex* add;
    ll index = findShareLine(cp, pl, ch, &add);
    if (index == -1) return false;

    int share_two_line = isShareTwoLine(cp, index, add); //before : 0, next : 1, no : -1
    if (share_two_line != -1)
    {
        cp->setMBB(pl);
        index += share_two_line;
        if (index >= (ll)cp->v_list.size())
        {
            index -= (ll)cp->v_list.size();
        }
        cp->sq_area += pl->getArea();
        cp->v_list.erase(cp->v_list.begin() + index);
        cp->av_normal = cp->av_normal + pl_nv;
        //cp->refreshNormal();

        return true;
    }

    if (checkMakeHole(cp, index, add)) return false;

    if (ch->isSamePlanar(pl_nv, cp->av_normal, 30.0 ))
    {
        cp->setMBB(pl);
        cp->av_normal = cp->av_normal + pl_nv;
        cp->sq_area += pl->getArea();
        cp->v_list.insert(cp->v_list.begin() + index + 1, add);
        //cp->refreshNormal();
        return true;
    }


    return false;
}

bool TriangleAttacher::isShareThreeLine(Surface* cp, ll index){
    ll n_index = index + 3;
    if (n_index >= (int)cp->v_list.size()) n_index -= cp->v_list.size();

    return (cp->v_list[index] == cp->v_list[n_index]);

}

ll TriangleAttacher::findShareLine(Surface* cp, Triangle* pl, Checker* ch, Vertex** add)
{
    for (ull id = 0 ; id < cp->v_list.size() ; id++)
    {
        ull n_id = id + 1;
        if (n_id == cp->v_list.size()) n_id = 0;

        if (cp->v_list[id] == pl->b){
            if (cp->v_list[n_id] == pl->a){
                *add = pl->c;
                return id;
            }
        }
        if (cp->v_list[id] == pl->c){
            if(cp->v_list[n_id] == pl->b){
                *add = pl->a;
                return id;
            }
        }
        if (cp->v_list[id] == pl->a){
            if(cp->v_list[n_id] == pl->c){
                *add = pl->b;
                return id;
            }
        }
    }
    return -1;
}

int TriangleAttacher::isShareTwoLine(Surface* cp, ll index, Vertex* add_id){
    ll b_index = index - 1;
    if (b_index < 0) b_index = cp->v_list.size() - 1;

    index += 2;
    if (index >= (ll)cp->v_list.size()){
        index -= cp->v_list.size();
    }

    if ( cp->v_list[b_index] == add_id ) return 0;
    else if ( cp->v_list[index] == add_id ) return 1;
    else return -1;
}


bool TriangleAttacher::checkMakeHole(Surface* cp, ll index, Vertex* add_id){
    for (ll i = 0 ; i < (ll)cp->v_list.size() ; i++)
    {
        if (cp->v_list[i] == add_id)
        {
            index += 2;
            if (index >= (ll)cp->v_list.size())
            {
                index -= cp->v_list.size();
            }
            if (index != i){
                return true;
            }
        }
    }
    return false;
}


bool TriangleAttacher::isNeighbor(Surface* cp, Triangle* pl){
    Vertex* v_list[3] = {pl->a, pl->b, pl->c};

    for (int i = 0 ; i < 3 ; i++){
        if (cp->isInMBB(v_list[i])){
            return true;
        }
    }

    return false;
}

