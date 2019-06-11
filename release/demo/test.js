window.onload = function() 
{
    var baseUrl = "ws://localhost:12345";//设定请求端口
    var socket = new WebSocket(baseUrl);//发起连接

    socket.onclose = function() 
    {
        console.error("web channel closed");
    };
    socket.onerror = function(error) 
    {
        console.error("web channel error: " + error);
    };
    socket.onopen = function() 
    {
        new QWebChannel(socket, function(channel) 
        {
            window.core = channel.objects.bridge;//将端口和channel绑定
            
            $('a').click(function()
            {
//                  	var str=$(this).attr('href');
//                  	var str = "sound://uk/dog__gb_1.spx";
				// var patt = /(.*?):\/\/(.*)/;在这里执行拆分 到qt端仍需再次拆分 不如直接在qt
				//var res=patt.exec(str);
				//console.log($(this).attr('href'))
				core.receiveText($(this).attr('href'));//将href发送给qt
				//console.log($(this).attr('href'));
				return false
			});
			$('span').dblclick(function()
            {
//                  	var str=$(this).attr('href');
//                  	var str = "sound://uk/dog__gb_1.spx";
				// var patt = /(.*?):\/\/(.*)/;在这里执行拆分 到qt端仍需再次拆分 不如直接在qt
				//var res=patt.exec(str);
				//console.log($(this).attr('href'))
				var txt = window.getSelection().toString();//获取选中内容
				core.dealselect(txt);
				event.stopPropagation();//停止冒泡 不触发父级span
			});
                
            // core.receiveText("clientready");
         }); 
     }
 }