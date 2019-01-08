// pages/outdoor/outdoor-future/outdoor-future.js

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
    var location = options.location;
    var that = this;
    // console.log(location);
    var url = 'http://wthrcdn.etouch.cn/weather_mini?city=' + location ;
    wx.request({
      url: url,
      method: "GET",
      header: {
        "content-type": "json"
      },
      success: function (res) {
        // console.log(res.data.data)
        that.draw(res.data.data);
      },
      fail: function (error) {
        console.log(error)
      }
    })



  },


  draw:function(resData){
    var xDate = ["昨天","今天","明天","后天"];
    var high = [];
    var low = [];


    high.push(resData.yesterday.high.split(" ")[1].replace("℃", ''));
    low.push(resData.yesterday.low.split(" ")[1].replace("℃", ''));
    
    for(var i=0;i<resData.forecast.length;i++){
      if(i>2){
        xDate.push(resData.forecast[i].date.split("星")[0]);
      }
      high.push(resData.forecast[i].high.split(" ")[1].replace("℃", ''));
      low.push(resData.forecast[i].low.split(" ")[1].replace("℃", ''));
    }
    

    // console.log(xDate);console.log(high);console.log(low);
    var minVal = Math.min.apply(null, low);
    var windowWidth = wx.getSystemInfoSync().windowWidth  
    new wxCharts({
        canvasId: 'areaCanvas',
        type: 'area',
        categories: xDate,
        animation: true,
        background: '#f5f5f5',
        series: [{
            name: '最高温度',
            data: high,
            color: "yellow",
            format: function (val, name) {
                return val + '℃';
            }
        }, {
            name: '最低温度',
            data: low,
            color: "#AEEEEE",
            format: function (val, name) {
                return val + '℃';
            }
        }],
        xAxis: {
            disableGrid: true
        },
        yAxis: {
            title: '近五天气温状况（℃）',
            format: function (val) {
                return val;
            },
            min: minVal-5,
        },
        width: windowWidth,
        height: 500,
        animation:true,
        legend:true,
        dataLabel: true,
        dataPointShape: true,
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