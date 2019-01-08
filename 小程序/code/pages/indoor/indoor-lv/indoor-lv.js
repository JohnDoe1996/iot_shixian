// pages/indoor/indoor-lv/indoor-lv.js
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
      url: "http://johndoeisbeginner.top:8888/api?choose=lv&limit=" + limit,
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
    var limit = 20;
    var lvData = {
      l:{
        titleList: [],
        dataList: []
      },
      v:{
        dataTitle: 0,
        dataList: [],
      }
    }
    for(var i=0;i<limit;i++){
        lvData.l.titleList.push(" ");
    }
    lvData.l.dataList = res.lData;
    lvData.v.dataTitle = res.vData;
    lvData.v.dataList[0] = res.vData;
    lvData.v.dataList[1] = 100-res.vData;

    this.columnDraw(lvData);
    this.ringDraw(lvData);
  },

  columnDraw: function (value){
    var windowWidth = wx.getSystemInfoSync().windowWidth;
    new wxCharts({
        canvasId: 'columnCanvas',
        type: 'column',
        animation: true,
        categories: value.l.titleList,
        series: [{
            name: '光照强度比(%)',
            data:  value.l.dataList,
            format: function (val, name) {
                return val + '%';
            }
        }],
        yAxis: {
            format: function (val) {
                return val + '%';
            },
            min: 0
        },
        xAxis: {
            disableGrid: true,
        },
        extra: {
            column: {
                width: 20
            }
        },
        width: windowWidth,
        height: 200,
    });
  },

  ringDraw: function(value){
    var windowWidth = wx.getSystemInfoSync().windowWidth;
    new wxCharts({
        animation: true,
        canvasId: 'ringCanvas',
        type: 'ring',
        extra: {
            ringWidth: 25,
            pie: {
                offsetAngle: -45
            }
        },
        title: {
            name: value.v.dataTitle + '%',
            color: '#7cb5ec',
            fontSize: 25
        },
        subtitle: {
            name: '噪声占比',
            color: '#666666',
            fontSize: 15
        },
        series: [{
            name: '噪声大占比',
            data: value.v.dataList[0],
            color: "#FFFF00",
            stroke: true
        }, {
            name: '噪声小占比',
            data: value.v.dataList[1],
            color: "#0F0",
            stroke: false
        }],
        disablePieStroke: true,
        width: windowWidth,
        height: 200,
        dataLabel: false,
        legend: false,
        padding: 0
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