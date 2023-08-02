class CBattleRoyale {
public:
    CBattleRoyale(unsigned height, unsigned width, const std::list<CPos> &zones)
    {
        h=height;
        w=width;
        for(const auto & x : zones){
            zone[{x.m_posX,x.m_posY}]=x.m_hour;
        }
    }

    unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const
    {
        //Kontrola jestli nezačínám na zakázané zóně
        auto i = zone.find({xSt,ySt});
        if(i!=zone.end() && i->second==0){
            throw logic_error ("Path does not exist :(");
        }

        queue<pair<pair<int,int>,int>> q;
        set<pair<int,int>> visited;
        q.emplace(make_pair(xSt,ySt),0);
        visited.emplace(xSt,ySt);

        while(!q.empty()){
            auto curr = q.front();
            q.pop();
            if(curr.first.first==xEn && curr.first.second==yEn){
                return curr.second;
            }
            int xAdd [4] = {1,0,0,-1};
            int yAdd [4] = {0,1,-1,0};
            for(int i=0;i<4;i++){
                int currX = curr.first.first + xAdd[i];
                int currY = curr.first.second + yAdd[i];
                if(currX>=0 && currX<w && currY>=0 && currY<h && visited.count({currX,currY})==0){
                    auto it = zone.find({currX,currY});
                    if(it==zone.end() || it->second>(curr.second+1)){
                        q.emplace(make_pair(currX,currY),curr.second+1);
                        visited.emplace(currX,currY);
                    }
                }
            }
        }
        throw logic_error ("Path does not exist :(");
    }

private:
    //      xPos,yPos,time
    map<pair<int,int>,int> zone;
    int h; //height
    int w; //width
};