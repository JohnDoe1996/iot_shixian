// 天气类抽离方法
module.exports = {
	dealTemperature: (t) => {
		t = t.replace(/\s/g, '');
		var arr = t.split('~');
    t = arr[0] + '℃/' + arr[1];
		return t;
	},
	weatherLevel: (str) => {
		var arr = ["晴", "多云", "阴", "阵雨", "雷阵雨", "雷阵雨伴有冰雹",
			"雨夹雪", "小雨", "中雨", "大雨", "暴雨", "大暴雨", "特大暴雨", "阵雪",
			"小雪", "中雪", "大雪", "暴雪", "雾", "冻雨", "沙尘暴", "小雨转中雨",
			"中雨转大雨", "大雨转暴雨", "暴雨转大暴雨", "大暴雨转特大暴雨", "小雪转中雪",
			"中雪转大雪", "大雪转暴雪", "浮尘", "扬沙", "强沙尘暴", "霾","大雨转多云"
		];
    var index = arr.indexOf(str.split('转')[0]) + 1;
    return '../../images/weather/' + index + '.png';
	},
	weatherMoreLevel: (str) => {
		var strs = str.split('转'),
			result = [];
		for (var i = 0; i < strs.length; i++) {
			result.push({
				src: module.exports.weatherLevel(strs[i])
			});
		}
		return result;
	},
	returnStyle: function(str) {
		if (str.indexOf('雪') > 0) {
			//下雪
			return '';
		}
		if (str.indexOf('雨') > 0) {
			//雨天
			return 'rain';
		}
		if (str.indexOf('晴') > 0) {
			//晴天
			return 'sunny';
		}
		if (str.indexOf('云') > 0) {
			//多云
			return 'cloudy';
		}
		return 'default';
	}
}