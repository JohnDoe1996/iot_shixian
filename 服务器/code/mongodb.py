from pymongo import MongoClient
import datetime


class database:
    conn = MongoClient("119.23.19.129", 27017)
    db = conn.iot

    def addOne(self,t,h,l,v):
        dataDict = {
            "datetime": datetime.datetime.now(),
            "t": t,
            "h": h,
            "l": l,
            "v": v
        }
        self.db.data.insert(dataDict)

    def findAll(self,limit):
        tList = []
        hList = []
        lList = []
        vList = [0,0]
        find = self.db.data.find(
                                {},
                                sort=[("datetime",-1)],
                                limit=limit
                                )
        for i in find:
            tList.append(int(i["t"]))
            hList.append(int(i["h"]))
            lList.append(int(i["l"]))
            if i["v"] == 0 :
                vList[0] += 1
            else:
                vList[1] += 1
        print(tList)
        print(hList)
        print(lList)
        print(vList)
        return (tList,hList,lList,vList)



if __name__ == "__main__":
    d = database()
    d.addOne(29,31,68,0)
    d.findAll(20)





