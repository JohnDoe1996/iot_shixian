// pages/indoor/indoor-th/indoor-th.js
var wxCharts = require('../../../libs/wxcharts.js');

Page({

  /**
   * 页面的初始数据
   */
  data: {
  
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var limit = 20;
    var that = this;
    wx.request({
      url: "http://johndoeisbeginner.top:8888/api?choose=th&limit=" + limit,
      method: "GET",
      header:{
        "content-type":"json"
      },
      success:function(res){
        console.log(res);
        that.deal(res.data);
      },
      fail:function(err){
        console.log(err);
      }
    });
  },

  deal:function(res){
    var limit = 20
    var thData = {
      title : [],
      tList : [],
      hList : []
    };
    for(var i=0;i<limit;i++){
      thData.title.push(" ");
      // thData.tList.push(res.tData[i]);
      // thData.hList.push(res.hData[i]);
    }
    thData.hList = res.hData;
    thData.tList = res.tData;
    this.columnDraw(thData);
  },

  columnDraw: function(value){
    var windowWidth = wx.getSystemInfoSync().windowWidth  
    new wxCharts({
        canvasId: 'lineCanvas',
        type: 'line',
        categories: value.title,
        animation: true,
        background: '#f5f5f5',
        series: [{
            name: '温度(℃)',
            data: value.tList,
            format: function (val, name) {
                return val;
            }
        }, {
            name: '湿度(%RH)',
            data: value.hList,
            format: function (val, name) {
                return val;
            }
        }],
        xAxis:{
          disableGrid: true,
        },
        yAxis: {
          format: function (val) {
              return val;
          },
        // min: 0
        },
        width: windowWidth,
        height: 450,
        dataLabel: false,
        dataPointShape: false,
        extra: {
            lineStyle: 'curve'
        }
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