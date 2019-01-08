// pages/outdoor/outdoor.js
var bmap = require('../../libs/bmap-wx.js');
var tools = require('../../utils/tools.js');
var utils = require('./utils/util.js');
var location = " ";

Page({

  /**
   * 页面的初始数据
   */
  data: {
    style: '',
    show: 'hide',
    mapIconSrc: '../../images/weather/map.png',
    todyWeather: '', //今天天气
    futureThreeDay: [], //未来三天
    variousIndex: [] //各项指数
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    try{
      this.data.todyWeather = that.addItemData(wx.getStorageSync('today'));
      this.data.futureThreeDay = wx.getStorageSync('future');
      this.data.variousIndex = wx.getStorageSync('index');
    }catch(e){
      console.log(e);
    }
    this.loadBaidu();
  },

// 请求百度天气接口
  loadBaidu: function(){
    var that = this;
    tools.loading("加载中...");
    var BMap = new bmap.BMapWX({
      ak: 'Iqvj1uX3TRKsYDfF3kZN94dxHiGeLRwQ'//'g4I2oOxpdnhxmuQwYaDrrLayDqZBft78'
    });
    var fail = function (data) {
      tools.loadingEnd();
      tools.errorDialog('数据获取失败，重新加载', query);
    };
    var success = function(data){
      //处理数据，返回自定义格式数据
      var tody = that.dealTodayData(data.currentWeather[0]);
      var future = that.dealFuture(data.originalData.results[0].weather_data);
      var index = that.dealIndex(data.originalData.results[0].index);
      // console.log(data.originalData.results[0].index);
      that.setData({
        show: 'show',
        todyWeather: that.addItemData(tody),
        futureThreeDay: future,
        variousIndex: index
      })
      location = tody.city;
      wx.setStorageSync('today', tody);
      wx.setStorageSync('future', future);
      wx.setStorageSync('index', index);
      tools.loadingEnd();
    }
    var query = function(){
      // 发起weather请求 
      BMap.weather({
        fail: fail,
        success: success
      });
    }
    query();
  },


  //处理今天天气的数据
  dealTodayData: function (data) {
    var date = data.date.split('(')[0];
    var now = parseInt(data.date.split('：')[1].replace(/[\(-\)]/g, '')) + '℃';
    var result = {
      city: data.currentCity,
      pm25: data.pm25,
      date: date,
      realtimeTemperature: now,
      temperature: utils.dealTemperature(data.temperature),
      weather: data.weatherDesc,
      wind: data.wind,
      iconSrc: utils.weatherLevel(data.weatherDesc),
    };
    return result;
  },

  //处理未来天气的数据
  dealFuture: function (data) {
    var result = [];
    for (var i = 1; i < data.length; i++) {
      var item = {
        weather: data[i].weather,
        date: data[i].date,
        temperature: utils.dealTemperature(data[i].temperature),
        iconSrc: utils.weatherMoreLevel(data[i].weather)
      };
      result.push(item);
    }
    return result;
  },

  //处理索引数据
  dealIndex: (data) => {
    var result = [];
    for (var i = 0; i < data.length; i++) {
      var item = {
        title: data[i].title,
        value: data[i].zs,
        desc: data[i].des
      };
      result.push(item);
    }
    return result;
  },

  // 返回背景颜色，并设置背景色
  addItemData: function (item) {
    item.style = utils.returnStyle(item.weather);
    return item;
  },

// 点击今天的天气跳转，查看相关指数
  onToday: function(){
    wx.navigateTo({
      url: "./outdoor-today/outdoor-today"
    })
  },

  // 点击未来天气跳转，查看天气列表
  onFuture: function () {
    wx.navigateTo({
      url: "./outdoor-future/outdoor-future?location=" + location,
    })
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
    this.data.todyWeather = ''; //今天天气
    this.data.futureThreeDay = []; //未来三天
    this.data.variousIndex = [];//各项指数
    this.loadBaidu();
    wx.showNavigationBarLoading();
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