// pages/indoor/indoor.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    userName: " ",
    userLogo: " ",
    switchTurn: [false,false,false],
    tValue: 0,
    hValue: 0,
    lValue: 0,
    vValue: "low↓", //"loud↑"
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var userInfo = wx.getStorageSync("userInfo");
    // console.log(userInfo.avatarUrl);
    this.setData({
      userName: userInfo.nickName,
      userLogo: userInfo.avatarUrl,
    });
    var that = this;
    wx.connectSocket({
      url: 'ws://johndoeisbeginner.top:8888/chat'//'ws://localhost:8005/chat'
    })
    wx.onSocketOpen(res => {
      wx.sendSocketMessage({
        data: "check_all",
      })
    })
    wx.onSocketMessage(function(res){
      console.log(res);
      switch(res.data.split(" ")[0]){
        case "all":
          that.dealAll(res.data);
          break;
      }
    })
  },

  onTurnSwitch: function (event) {
    var index = event.currentTarget.dataset.postId;
    // console.log(index);
    this.data.switchTurn[index] = !this.data.switchTurn[index];
    // this.setData({
    //   switchTurn: this.data.switchTurn
    // });
    wx.sendSocketMessage({
      data: "set_switch "+
            (this.data.switchTurn[0]?1:0)+" "+
            (this.data.switchTurn[1]?1:0)+" "+
            (this.data.switchTurn[2]?1:0),
    });
    // this.setData({
    //   switchTurn: this.data.switchTurn
    // });
    // console.log(index + ":" +this.data.switchTurn[index]);
  },

  dealAll: function(context){
      var switchData = [];
      for(var i=1;i<=3;i++){
        if (context.split(" ")[i]==0)
          switchData.push(false);
        else switchData.push(true);
      }
      this.setData({
        switchTurn: switchData,
        tValue: context.split(" ")[4],
        hValue: context.split(" ")[5],
        lValue: context.split(" ")[6],
        vValue: (context.split(" ")[7]==0)?"low↓":"loud↑"
      }); 
  },

  

  onJumpTH: function(){
    wx.navigateTo({
      url: "indoor-th/indoor-th"
    });
  },

  onJumpLV: function(){
    wx.navigateTo({
      url: "indoor-lv/indoor-lv"
    });
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
  
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
  
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
  
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
  
  }
})