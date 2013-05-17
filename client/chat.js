var tabactive = 0;
var tabs = new Array();
var roomcontents = new Array();
var rooms = {};
var roomnames = new Array();
var myname = "KASJS";
var mycolor = "BADF0D";
var colors = {};
var motds = {};
var toghost = null;
var loginstate = -1;
var titleshouldbe = "Coldstorm";
var mycountry = "";
var messagesunread = 0;
var fixtok = null;
var muted = false;
var starred = "";
var userips = {};
var myrank = {};
	
var yts=new Array();
	
function getCountryName(strCountryCode) {
    for (var i = 0; i < countries.length; i++) {  
        if (strCountryCode == countries[i].code) {
            return countries[i].name;
        }
    }
    return "";
}

var Connection = new Object();
Connection.Connect = function(target,onmsg,onjoin,onquit){
	this.tgt = target;
	this.session="";
	this.onm = onmsg;
	this.onj = onjoin;
	this.onq = onquit;
	
	
	var xmlhttp;
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp=new XMLHttpRequest();
	}
	else
	{// code for IE6, IE5
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange=function(t=this)
	{
		if (xmlhttp.readyState==4 && xmlhttp.status==200)
		{
			t.session=xmlhttp.responseText;
			this.onj();
		}
	}
	xmlhttp.open("GET",target+"/ajax.php?session=new",true);
	xmlhttp.send();
	
	
	
	this.interval = setInterval(
		function(tp=this){
			var xmlhttp1;
			if (window.XMLHttpRequest)
			{// code for IE7+, Firefox, Chrome, Opera, Safari
				xmlhttp1=new XMLHttpRequest();
			}
			else
			{// code for IE6, IE5
				xmlhttp1=new ActiveXObject("Microsoft.XMLHTTP");
			}
			xmlhttp1.onreadystatechange=function(to=tp)
			{
				if (xmlhttp1.readyState==4 && xmlhttp1.status==200)
				{
					to.onm(xmlhttp1.responseText);
					xmlhttp1.open("GET",target+"/ajax.php?session="+tp.session+"&control=poll&id="+Math.random(),true);
					xmlhttp1.send();
				}
				if (xmlhttp1.readyState==4 && xmlhttp1.status==404){
					to.session="";
					to.onq();
					clearInterval(to.interval);
				}
				
			}
			xmlhttp1.open("GET",target+"/ajax.php?session="+tp.session+"&control=poll&id="+Math.random(),true);
			xmlhttp1.send();
		}, 25000 );
}
Connection.close = function(){
	this.onq();
	clearInterval(this.interval);
	var xmlhttp11;
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp11=new XMLHttpRequest();
	}
	else
	{// code for IE6, IE5
		xmlhttp11=new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp11.open("GET",target+"/ajax.php?session="+tp.session+"&control=close&id="+Math.random(),true);
	xmlhttp11.send();
}
Connection.send = function(data){
	var xmlhttp12;
	if (window.XMLHttpRequest)
	{// code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp12=new XMLHttpRequest();
	}
	else
	{// code for IE6, IE5
		xmlhttp12=new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp12.open("GET",target+"/ajax.php?session="+tp.session+"&control=post&id="+Math.random()+"&data="+dta,true);
	xmlhttp12.send();
}



// ISO 3166-1 country names and codes from http://opencountrycodes.appspot.com/javascript              
countries = [     {code: "GB", name: "United Kingdom"},     {code: "AF", name: "Afghanistan"},     {code: "AX", name: "Aland Islands"},     {code: "AL", name: "Albania"},     {code: "DZ", name: "Algeria"},     {code: "AS", name: "American Samoa"},     {code: "AD", name: "Andorra"},     {code: "AO", name: "Angola"},     {code: "AI", name: "Anguilla"},     {code: "AQ", name: "Antarctica"},     {code: "AG", name: "Antigua and Barbuda"},     {code: "AR", name: "Argentina"},     {code: "AM", name: "Armenia"},     {code: "AW", name: "Aruba"},     {code: "AU", name: "Australia"},     {code: "AT", name: "Austria"},     {code: "AZ", name: "Azerbaijan"},     {code: "BS", name: "Bahamas"},     {code: "BH", name: "Bahrain"},     {code: "BD", name: "Bangladesh"},     {code: "BB", name: "Barbados"},     {code: "BY", name: "Belarus"},     {code: "BE", name: "Belgium"},     {code: "BZ", name: "Belize"},     {code: "BJ", name: "Benin"},     {code: "BM", name: "Bermuda"},     {code: "BT", name: "Bhutan"},     {code: "BO", name: "Bolivia"},     {code: "BA", name: "Bosnia and Herzegovina"},     {code: "BW", name: "Botswana"},     {code: "BV", name: "Bouvet Island"},     {code: "BR", name: "Brazil"},     {code: "IO", name: "British Indian Ocean Territory"},     {code: "BN", name: "Brunei Darussalam"},     {code: "BG", name: "Bulgaria"},     {code: "BF", name: "Burkina Faso"},     {code: "BI", name: "Burundi"},     {code: "KH", name: "Cambodia"},     {code: "CM", name: "Cameroon"},     {code: "CA", name: "Canada"},     {code: "CV", name: "Cape Verde"},     {code: "KY", name: "Cayman Islands"},     {code: "CF", name: "Central African Republic"},     {code: "TD", name: "Chad"},     {code: "CL", name: "Chile"},     {code: "CN", name: "China"},     {code: "CX", name: "Christmas Island"},     {code: "CC", name: "Cocos (Keeling) Islands"},     {code: "CO", name: "Colombia"},     {code: "KM", name: "Comoros"},     {code: "CG", name: "Congo"},     {code: "CD", name: "Congo, The Democratic Republic of the"},     {code: "CK", name: "Cook Islands"},     {code: "CR", name: "Costa Rica"},     {code: "CI", name: "Côte d'Ivoire"},     {code: "HR", name: "Croatia"},     {code: "CU", name: "Cuba"},     {code: "CY", name: "Cyprus"},     {code: "CZ", name: "Czech Republic"},     {code: "DK", name: "Denmark"},     {code: "DJ", name: "Djibouti"},     {code: "DM", name: "Dominica"},     {code: "DO", name: "Dominican Republic"},     {code: "EC", name: "Ecuador"},     {code: "EG", name: "Egypt"},     {code: "SV", name: "El Salvador"},     {code: "GQ", name: "Equatorial Guinea"},     {code: "ER", name: "Eritrea"},     {code: "EE", name: "Estonia"},     {code: "ET", name: "Ethiopia"},     {code: "FK", name: "Falkland Islands (Malvinas)"},     {code: "FO", name: "Faroe Islands"},     {code: "FJ", name: "Fiji"},     {code: "FI", name: "Finland"},     {code: "FR", name: "France"},     {code: "GF", name: "French Guiana"},     {code: "PF", name: "French Polynesia"},     {code: "TF", name: "French Southern Territories"},     {code: "GA", name: "Gabon"},     {code: "GM", name: "Gambia"},     {code: "GE", name: "Georgia"},     {code: "DE", name: "Germany"},     {code: "GH", name: "Ghana"},     {code: "GI", name: "Gibraltar"},     {code: "GR", name: "Greece"},     {code: "GL", name: "Greenland"},     {code: "GD", name: "Grenada"},     {code: "GP", name: "Guadeloupe"},     {code: "GU", name: "Guam"},     {code: "GT", name: "Guatemala"},     {code: "GG", name: "Guernsey"},     {code: "GN", name: "Guinea"},     {code: "GW", name: "Guinea-Bissau"},     {code: "GY", name: "Guyana"},     {code: "HT", name: "Haiti"},     {code: "HM", name: "Heard Island and McDonald Islands"},     {code: "VA", name: "Holy See (Vatican City State)"},     {code: "HN", name: "Honduras"},     {code: "HK", name: "Hong Kong"},     {code: "HU", name: "Hungary"},     {code: "IS", name: "Iceland"},     {code: "IN", name: "India"},     {code: "ID", name: "Indonesia"},     {code: "IR", name: "Iran, Islamic Republic of"},     {code: "IQ", name: "Iraq"},     {code: "IE", name: "Ireland"},     {code: "IM", name: "Isle of Man"},     {code: "IL", name: "Israel"},     {code: "IT", name: "Italy"},     {code: "JM", name: "Jamaica"},     {code: "JP", name: "Japan"},     {code: "JE", name: "Jersey"},     {code: "JO", name: "Jordan"},     {code: "KZ", name: "Kazakhstan"},     {code: "KE", name: "Kenya"},     {code: "KI", name: "Kiribati"},     {code: "KP", name: "Korea, Democratic People's Republic of"},     {code: "KR", name: "Korea, Republic of"},     {code: "KW", name: "Kuwait"},     {code: "KG", name: "Kyrgyzstan"},     {code: "LA", name: "Lao People's Democratic Republic"},     {code: "LV", name: "Latvia"},     {code: "LB", name: "Lebanon"},     {code: "LS", name: "Lesotho"},     {code: "LR", name: "Liberia"},     {code: "LY", name: "Libyan Arab Jamahiriya"},     {code: "LI", name: "Liechtenstein"},     {code: "LT", name: "Lithuania"},     {code: "LU", name: "Luxembourg"},     {code: "MO", name: "Macao"},     {code: "MK", name: "Macedonia, The Former Yugoslav Republic of"},     {code: "MG", name: "Madagascar"},     {code: "MW", name: "Malawi"},     {code: "MY", name: "Malaysia"},     {code: "MV", name: "Maldives"},     {code: "ML", name: "Mali"},     {code: "MT", name: "Malta"},     {code: "MH", name: "Marshall Islands"},     {code: "MQ", name: "Martinique"},     {code: "MR", name: "Mauritania"},     {code: "MU", name: "Mauritius"},     {code: "YT", name: "Mayotte"},     {code: "MX", name: "Mexico"},     {code: "FM", name: "Micronesia, Federated States of"},     {code: "MD", name: "Moldova"},     {code: "MC", name: "Monaco"},     {code: "MN", name: "Mongolia"},     {code: "ME", name: "Montenegro"},     {code: "MS", name: "Montserrat"},     {code: "MA", name: "Morocco"},     {code: "MZ", name: "Mozambique"},     {code: "MM", name: "Myanmar"},     {code: "NA", name: "Namibia"},     {code: "NR", name: "Nauru"},     {code: "NP", name: "Nepal"},     {code: "NL", name: "Netherlands"},     {code: "AN", name: "Netherlands Antilles"},     {code: "NC", name: "New Caledonia"},     {code: "NZ", name: "New Zealand"},     {code: "NI", name: "Nicaragua"},     {code: "NE", name: "Niger"},     {code: "NG", name: "Nigeria"},     {code: "NU", name: "Niue"},     {code: "NF", name: "Norfolk Island"},     {code: "MP", name: "Northern Mariana Islands"},     {code: "NO", name: "Norway"},     {code: "OM", name: "Oman"},     {code: "PK", name: "Pakistan"},     {code: "PW", name: "Palau"},     {code: "PS", name: "Palestinian Territory, Occupied"},     {code: "PA", name: "Panama"},     {code: "PG", name: "Papua New Guinea"},     {code: "PY", name: "Paraguay"},     {code: "PE", name: "Peru"},     {code: "PH", name: "Philippines"},     {code: "PN", name: "Pitcairn"},     {code: "PL", name: "Poland"},     {code: "PT", name: "Portugal"},     {code: "PR", name: "Puerto Rico"},     {code: "QA", name: "Qatar"},     {code: "RE", name: "Réunion"},     {code: "RO", name: "Romania"},     {code: "RU", name: "Russian Federation"},     {code: "RW", name: "Rwanda"},     {code: "BL", name: "Saint Barthélemy"},     {code: "SH", name: "Saint Helena"},     {code: "KN", name: "Saint Kitts and Nevis"},     {code: "LC", name: "Saint Lucia"},     {code: "MF", name: "Saint Martin"},     {code: "PM", name: "Saint Pierre and Miquelon"},     {code: "VC", name: "Saint Vincent and the Grenadines"},     {code: "WS", name: "Samoa"},     {code: "SM", name: "San Marino"},     {code: "ST", name: "Sao Tome and Principe"},     {code: "SA", name: "Saudi Arabia"},     {code: "SN", name: "Senegal"},     {code: "RS", name: "Serbia"},     {code: "SC", name: "Seychelles"},     {code: "SL", name: "Sierra Leone"},     {code: "SG", name: "Singapore"},     {code: "SK", name: "Slovakia"},     {code: "SI", name: "Slovenia"},     {code: "SB", name: "Solomon Islands"},     {code: "SO", name: "Somalia"},     {code: "ZA", name: "South Africa"},     {code: "GS", name: "South Georgia and the South Sandwich Islands"},     {code: "ES", name: "Spain"},     {code: "LK", name: "Sri Lanka"},     {code: "SD", name: "Sudan"},     {code: "SR", name: "Suriname"},     {code: "SJ", name: "Svalbard and Jan Mayen"},     {code: "SZ", name: "Swaziland"},     {code: "SE", name: "Sweden"},     {code: "CH", name: "Switzerland"},     {code: "SY", name: "Syrian Arab Republic"},     {code: "TW", name: "Taiwan, Province of China"},     {code: "TJ", name: "Tajikistan"},     {code: "TZ", name: "Tanzania, United Republic of"},     {code: "TH", name: "Thailand"},     {code: "TL", name: "Timor-Leste"},     {code: "TG", name: "Togo"},     {code: "TK", name: "Tokelau"},     {code: "TO", name: "Tonga"},     {code: "TT", name: "Trinidad and Tobago"},     {code: "TN", name: "Tunisia"},     {code: "TR", name: "Turkey"},     {code: "TM", name: "Turkmenistan"},     {code: "TC", name: "Turks and Caicos Islands"},     {code: "TV", name: "Tuvalu"},     {code: "UG", name: "Uganda"},     {code: "UA", name: "Ukraine"},     {code: "AE", name: "United Arab Emirates"},     {code: "GB", name: "United Kingdom"},     {code: "US", name: "United States"},     {code: "UM", name: "United States Minor Outlying Islands"},     {code: "UY", name: "Uruguay"},     {code: "UZ", name: "Uzbekistan"},     {code: "VU", name: "Vanuatu"},     {code: "VE", name: "Venezuela"},     {code: "VN", name: "Viet Nam"},     {code: "VG", name: "Virgin Islands, British"},     {code: "VI", name: "Virgin Islands, U.S."},     {code: "WF", name: "Wallis and Futuna"},     {code: "EH", name: "Western Sahara"},     {code: "YE", name: "Yemen"},     {code: "ZM", name: "Zambia"},     {code: "ZW", name: "Zimbabwe"},
    {code: "QQ", name: "Outlaw"},
    {code: "HE", name: "Holy Roman Empire"},
    {code: "GO", name: "Gondor"},
    {code: "OZ", name: "Land of Oz"},
    {code: "HY", name: "Hyrule"}
    
    
];

	
function set_cookie(c_name,value)
{
	var exdate=new Date();
	exdate.setDate(exdate.getDate() + 2000);
	var c_value=escape(value) +"; expires="+exdate.toUTCString();
	document.cookie=c_name + "=" + c_value;
}
function get_cookie(c_name)
{
	var i,x,y,ARRcookies=document.cookie.split(";");
	for (i=0;i<ARRcookies.length;i++)
	{
		x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
		y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
		x=x.replace(/^\s+|\s+$/g,"");
		if (x==c_name)
		{
			return unescape(y);
		}
	}
	return null;
}

function dopopup( title, content, btn ){
	document.getElementById("popup").style.top = "50%";
	document.getElementById("popup").innerHTML = "<h1>"+title+"</h1><br/><h2>"+content+"</h2>"+"<div id=\"dismissbutton\"><button id=\"dbutt\">"+btn+"</button></div>";
	document.getElementById("dbutt").disabled = false;
	document.getElementById("dbutt").onclick = function(){dismisspopup();}
}
function dismisspopup( ){
	document.getElementById("popup").style.top = "-2000px";
	document.getElementById("dbutt").disabled = true;
}

	
	
var users = new Array();
ws = null;
function Send(val) {
    //msg("Sending: " + $D('sendText').value);
    ws.send(unescape(encodeURIComponent(val)));
	console.log( val );
};
function htmlEntities(str) {
    return String(str).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
}
	
function updateuserlist( ind ){
	if( tabs.length <= 0 ) return false;
	if( ind != tabactive ) return false;

	
	var st = "";
	if( users[ind] == null ) return;
	for( var i = 0; i < users[ind].length; ++i ){
		var cl = users[ind][i];
		if( "+#@~".indexOf(cl[0]) >= 0 )
			cl = cl.substring(1);
		st += "<p style=\"cursor:pointer;\" ondblclick=\"startpm('"+users[ind][i]+"')\">";
		var tat = 0;
		if( colors[cl.toLowerCase()] != null ){
			if( colors[cl.toLowerCase()].length >= 8 ){
				var ttt = htmlEntities(colors[cl.toLowerCase()].substring(6,8));
				if( ttt!="" ){ st += "<img class=\"flag\" src=\"flags/"+ttt.toLowerCase()+".png\" title=\""+getCountryName(ttt.toUpperCase())+"\"/> "; }
				else tat = 1;
			}
			else tat = 1;
		}
		else{
			tat = 1;
		}
		if( tat == 1 ) st += "<img class=\"flag\" src=\"flags/qq.png\" title=\""+getCountryName("QQ")+"\"/> ";
		
		st += "<span id=\"text\"><span style=\"";
		if( colors[cl.toLowerCase()] != null ){
			if( colors[cl.toLowerCase()].length >= 6 ){
				var ttt = htmlEntities(colors[cl.toLowerCase()].substring(0,6));
				if( ttt!="" ){ st += "color:#"+ttt+";"; }
			}
		}
		st += "\">" + users[ind][i];
		if( starred.toLowerCase() == users[ind][i].toLowerCase() )
			st += "<span id=\"star\">*</span>";
		st += "</span></span></p>";
	}
	var t = document.getElementById("userbox");
	t.innerHTML = st;
}
function Join(room){
	Send( "JOIN "+room+"\r\n" );
}
function parseraw( contents, changehtml ){
	contents = htmlEntities(contents).replace(/(http|https|irc|mailto)(:\/\/[^ ]*)/g,"<a href=\"$1$2\" target=\"_blank\" class=\"color\" >$1$2</a>");
	contents = contents.replace(/( |$)(#[^ ^]*)/, "$1<span style=\"cursor:pointer;\" onclick=\"Join('$2')\">$2</span>" );
	if( changehtml == true )
		contents = contents.replace(/<a href="([^"]*)(\.png|\.tif|\.jpg|\.jpeg|\.bmp|\.gif)"([^>]*)>([^>]*)>/, "<span id=\"imgnum"+imgnumber+"\"><a href=\"javascript:void(0);\" onclick=\"makeimage('this', '$1$2', 'imgnum"+imgnumber+"');this.preventDefault();return false;\" class=\"color\" >$4>" );
	contents = contents.replace(/<a href="([^"]*)(youtube\.com|youtu.be)(.*\?v=)([^&#\s"]*)([^"]*)"([^>]*)>([^>]*)>/, "<span id=\"imgnum"+imgnumber+"\"><a href=\"javascript:void(0);\" onclick=\"makeyttab('this', '$1$2$3$4$5', 'imgnum"+imgnumber+"', '$4');this.preventDefault();return false;\" class=\"color\" >$7>" );
	
	
	imgnumber++;
	if( contents.substring(0,4) == "&gt;" ){
		contents = "<span id=\"greentext\">"+contents+"</span>";
	}
	contents = contents.replace(/([0-9]{1,2})([^\n]*)/gi, "<span id=\"c$1\">$2</span>" );
	var rx = new RegExp( "(^|[^\\d\\w])"+myname+"([^\\d\\w]|$)", "gi" );
	if( contents.match(rx) != null ) {
		var pinged = false;
		if( muted == true )
			pinged = true;
		var inurl = false;
		var start = 0;
		var outp = "";
		var inspoiler=-1;
		for( var i = 0; i < contents.length; ++i ){
			if( inurl == 0 && contents.substring(i,i+myname.length).toLowerCase() == myname.toLowerCase() ){
				var t1 = contents.substring(i-1,i);
				var t2 = contents.substring(i+myname.length,i+myname.length+1);
				console.log("\""+t1+"\"  \""+t2+"\" ");
				if( ( t1 == "" || t1.match(/[^\d\w#]/gi) != null ) && 
				(t2 == "" || t2.match(/[^\d\w#]/gi) != null) ){
					if( pinged == false ){
						try{document.getElementById("ping").play();}catch(e){}
						pinged = true;
						titleshouldbe = roomnames[tabactive] + " - Coldstorm";
						titleshouldbe = "Coldstorm";
						
						document.title = "*** | " + titleshouldbe;
					}
					outp += contents.substring( start, i );
					outp += "<span id=\"highlight\">"+myname+"</span>";
					i += myname.length;
					start = i;
				}
			}
			if( inurl == false && contents.substring(i,i+myname.length) == "::" ){
				if( inspoiler == -1 ){
					inspoiler=i;
				}
				else {
					var v = contents.substring(inspoiler+2, i-1);
					outp += contents.substring( start, i );
					outp += "<span id=\"spoiler\">"+myname+"</span>";
				}
			}
			if( contents.substring(i,i+"<a href=".length).toLowerCase() == "<a href=".toLowerCase() ){
				inurl++;
			}
			if( contents.substring(i,i+"</a>".length).toLowerCase() == "</a>".toLowerCase() ){
				inurl--;
			}
		}
		outp += contents.substring( start );
		contents=outp;

	}
	{
		var inurl = 0;
		var start = 0;
		var outp = "";
		var inspoiler=-1;
		for( var i = 0; i < contents.length; ++i ){
			console.log(i);
			if( inurl == 0 && contents.substring(i,i+2) == "::" ){
				console.log(i);
				if( inspoiler == -1 ){
					inspoiler=i;
				}
				else {
					var v = contents.substring(inspoiler+2, i);
					outp += contents.substring( start, inspoiler );
					outp += "<span id=\"spoiler\">"+ v +"</span>";
					inspoiler = -1;
					start = i + 2;
					i += 2;
				}
			}
			if( contents.substring(i,i+"<a href=".length).toLowerCase() == "<a href=".toLowerCase() ){
				inurl++;
			}
			if( contents.substring(i,i+"</a>".length).toLowerCase() == "</a>".toLowerCase() ){
				inurl--;
			}
		}
		outp += contents.substring( start );
		contents=outp;
	}
	return contents;
}

function setmotd( channel, text ){
	motds[channel.toLowerCase()] = parseraw( text, false );
	if( tabactive == rooms[channel.toLowerCase()] ) document.getElementById("motd").innerHTML = motds[channel.toLowerCase()];
}

function addtab( text ){
text = htmlEntities(text);
    var e = document.createElement("div");
    e.setAttribute("id", "tab" );
    e.setAttribute("class", "inactive" );
    if( tabs.length == 0 )
        e.setAttribute("class", "active" );
    document.getElementById("tabline").appendChild(e);
    e.innerHTML = "<a href=\"javascript:void(0);\" onclick=\"changetab("+tabs.length+")\"><span class=\"link\">"+text+"</span></a>";
    tabs[tabs.length] = e;
	
    return tabs.length-1;
}
function addpmtab( text ){
	text  = htmlEntities(text);
    var e = document.createElement("div");
    e.setAttribute("id", "tab" );
    e.setAttribute("class", "inactive" );
    if( tabs.length == 0 )
        e.setAttribute("class", "active" );
    document.getElementById("tabline").appendChild(e);
    e.innerHTML = "<a href=\"javascript:void(0);\" onclick=\"changetab("+tabs.length+")\"><span class=\"link\">"+text+" <a href=\"javascript:void(0);\" onclick=\"closetab("+tabs.length+")\">X</a></span></a>";
    tabs[tabs.length] = e;
    return tabs.length-1;
}
function scrolltobot(){
	var a = document.getElementById("textbox");
	var t = a.scrollHeight;
	a.scrollTop = t-a.clientHeight;
}
function changetab( val ){
    tabs[tabactive].setAttribute("class","inactive");
	/*if( roomnames[tabactive].toLowerCase().indexOf("youtube/") == 0 ){
		var iframe = document.getElementById('youtubee');
		var innerDoc = iframe.contentDocument || iframe.contentWindow.document;
		
		dopopup(innerDoc.getElementById("player").getDuration(),"lol","dismiss");
	}*/
    tabactive = val;
    tabs[tabactive].setAttribute("class","active");
	document.getElementById("textbox").innerHTML = roomcontents[tabactive];
	scrolltobot();
	updateuserlist(tabactive);
	if( roomnames[tabactive] != null ){
		if( motds[roomnames[tabactive].toLowerCase()] != null )
		document.getElementById("motd").innerHTML = motds[roomnames[tabactive].toLowerCase()];
		else
		document.getElementById("motd").innerHTML ="";
	}
	titleshouldbe = roomnames[tabactive] + " - Coldstorm";
	titleshouldbe = "Coldstorm";
	
	
	document.title = titleshouldbe;
	
	
    return false;
}
function closetab( val ){
	if( tabs[val] == null ) return;
	if( roomnames[val] != null )
		if( roomnames[val][0] == "#" ){
			Send("PART "+roomnames[val]+"\r\n");
		}
    if( val == tabactive ) {
        do{
        tabactive--;
        } while( tabs[tabactive] == null && tabactive >= 0 );
	if( tabs[tabactive] == null && tabactive<=0 ){
		tabactive = tabs.length;
		do{
			tabactive--;
		} while( tabs[tabactive] == null && tabactive >= 0 );
	}
	if( tabs[tabactive] == null && tabactive<=0 ){
		return;
	}
        tabs[tabactive].setAttribute("class","active");
	document.getElementById("textbox").innerHTML = roomcontents[tabactive];
	updateuserlist(tabactive);
	if( motds[roomnames[tabactive].toLowerCase()] != null )
	document.getElementById("motd").innerHTML = motds[roomnames[tabactive].toLowerCase()];
	else
	document.getElementById("motd").innerHTML ="";
	scrolltobot();        
    }
    var e = tabs[val];
    tabs[val] = null;
    rooms[roomnames[val]] = null;
    //for( i=val;i<tabs.length-1; i++)
    //    tabs[i] = tabs[i+1];
    if( e != null )
    e.parentNode.removeChild(e);
    //document.getElementById("tabline").removeChild( tabs[val] );
    /*for( i=val;i<tabs.length-1; i++)
        tabs[i] = tabs[i+1];*/

    return false;
}
function startgame( name, channel ){
	Send("PRIVMSG #coldstorm :\x01ACTION has started a game of " + name + ". Join " + channel + " to play!\r\n" );
	Send("JOIN "+channel+"\r\n");
}

//function adduser( name, color, countrycode ){
    
//}
function startpm(withwho){
	if( "~@#+".indexOf(withwho[0])>=0)
		withwho = withwho.substring(1);
	if( withwho == "" ) return false;
	if( rooms[withwho.toLowerCase()] == null ){
		rooms[withwho.toLowerCase()] = tabs.length;
		addpmtab(withwho);
		roomcontents[rooms[withwho.toLowerCase()]] = " ";
		roomnames[rooms[withwho.toLowerCase()]] = withwho.toLowerCase();
		users[rooms[withwho.toLowerCase()]] = new Array();
		
	}
	changetab(rooms[withwho.toLowerCase()]);
}

function send(){
    var xmlhttp;
    if (window.XMLHttpRequest)
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp=new XMLHttpRequest();
    }
    else
    {// code for IE6, IE5
        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
    }   
}


function adduser( rm, usr, noupdate ){
	usr = htmlEntities(usr);
	if( usr == "" ) return;
	if( users == null ) return;
	
	
	if( usr[0] == "%" ) usr = "#" + usr.substring(1);
	var ind = rooms[rm.toLowerCase()];
	if( users[ind] == null ) return;
	var b = 0;
	for( var i = 0; i < users[ind].length; ++i ){
		b = 0;

		for( var j = 0; j < users[ind][i].length; ++j ){
			var lookup =  "`^_|{}[]-\\ZzYyXxWwVvUuTtSsRrQqPpOoNnMmLlKkJjIiHhGgFfEeDdCcBbAa9876543210+#@~";
			if( j < usr.length ){
				var a = lookup.indexOf( usr[j] );
				var c = lookup.indexOf(users[ind][i][j]);
				if( a < c ) break;
				if( a == c ) continue;
			}
			b = 1;
			users[ind].splice(i,0,usr);
			break;
		}
		if( b ) break;
	}
	if( b == 0 ) users[ind].splice(users[ind].length,0,usr);
	updateuserlist(ind);
	if( "+#@~".indexOf(usr[0]) > 0 )
	usr = usr.substring(1);
	/*if( noupdate == 0 || noupdate == null ){
		console.log("WHOIS "+usr+"\r\n");
		Send("whois "+usr+"\r\n");
	}*/
}
function removeuser( rm, usr ){
	var ret = false;
	var ind = rooms[rm.toLowerCase()];
	if( users[ind] == null ) return;
	for( var i = 0; i < users[ind].length; ++i ){
		var t = users[ind][i];
		if( "+#@~".indexOf(users[ind][i][0]) >= 0 )
			t = users[ind][i].substring(1);
		if( t.toLowerCase() == usr.toLowerCase() ){
			users[ind].splice(i, 1);
			--i;
			ret = true;
		}
	}
	updateuserlist(ind);
	return ret;
}




function putmsg( room, contents, nop ){
	var rm;
	if( room != null )
		rm = rooms[room.toLowerCase()];
	else
		rm = tabactive;
	if( nop == null || nop == 0)
	roomcontents[rm] += "<p>"+contents+"</p>";
	else roomcontents[rm] += contents;
	if(rm == tabactive){
		var a = document.getElementById("textbox");
		var t = a.scrollHeight;
		if( nop == null || nop == 0)
		a.innerHTML += "<p>"+contents+"</p>";//= roomcontents[rm];
		else
		a.innerHTML += contents;//= roomcontents[rm];
		if( t - (a.scrollTop + a.clientHeight) < 20 ){
			a.scrollTop = a.scrollHeight-a.clientHeight;
		}			
	}
	else{
		
	}
	
}

function makeyttab( e, path, imgnum, vid ){
	if (window.XMLHttpRequest)
	  {// code for IE7+, Firefox, Chrome, Opera, Safari
	  xmlhttp=new XMLHttpRequest();
	  }
	else
	  {// code for IE6, IE5
	  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	  }
	xmlhttp.open("GET","http://gdata.youtube.com/feeds/api/videos/"+vid+"?v=2",false);
	xmlhttp.send();
	xmlDoc=xmlhttp.responseXML;
	  
	  
	var withwho = "Youtube/"+vid;
	  withwho = xmlDoc.getElementsByTagName("title")[0].childNodes[0].nodeValue;
	  if( withwho.length > 25 ) withwho = withwho.substring(0, 10)+" ... " + withwho.substring(withwho.length-10);
	
	if( rooms[withwho.toLowerCase()] == null ){
		rooms[withwho.toLowerCase()] = tabs.length;
		addpmtab(withwho);
		roomcontents[rooms[withwho.toLowerCase()]] = " ";
		roomnames[rooms[withwho.toLowerCase()]] = withwho.toLowerCase();
	}
	changetab(rooms[withwho.toLowerCase()]);
	putmsg( withwho, "<iframe id=\"youtubee\" width=\"100%\" height=\"100%\" src=\"https://www.youtube.com/embed/"+vid+"\" frameborder=\"0\" allowfullscreen></iframe>",1 );
	//"<div class=\"closebutton\" style=\"cursor:pointer;\" onclick=\"closeyttab();\">X</div>";
	
	//document.getElementById("youtube").innerHTML = "<iframe id=\"youtubee\" width=\"100%\" height=\"100%\" src=\"https://www.youtube.com/embed/"+vid+"\" frameborder=\"0\" allowfullscreen></iframe>"+
	//"<div class=\"closebutton\" style=\"cursor:pointer;\" onclick=\"closeyttab();\">X</div>";
	//document.getElementById("chat").style.marginLeft = "-805px";
	//document.getElementById("youtube").style.marginLeft = "5px";
	//document.getElementById("youtube").style.top = "50%";
	
	
}

function closeyttab(  ){
	document.getElementById("youtube").innerHTML = "";
	//document.getElementById("chat").style.marginLeft = "-400px";
	document.getElementById("youtube").style.top = "-2000px";
}
function hideyttab(  ){
	//document.getElementById("youtube").innerHTML = "";
	//document.getElementById("chat").style.marginLeft = "-400px";
	document.getElementById("youtube").style.top = "-2000px";
}
function wideyttab(  ){
	//document.getElementById("youtube").innerHTML = "";
	//document.getElementById("chat").style.marginLeft = "-400px";
	document.getElementById("youtube").style.top = "-2000px";
}




function AddZero(num) {
    return (num >= 0 && num < 10) ? "0" + num : num + "";
}
function putmsgstamped( room, contents ){
	var now = new Date();

	putmsg( room, "<span id=\"timestamp\">["+AddZero(now.getHours())+":"+AddZero(now.getMinutes())+"]</span><span id=\"spacer\"></span>"+contents);
}
function unmakeimage( e, path, im ){
	var a = document.getElementById("textbox");
	var t = a.scrollHeight;
	var tt = a.scrollTop;
	document.getElementById(im).innerHTML = "<span id=\""+im+"\"><a href=\"javascript:void(0);\" onclick=\"makeimage('"+e+"', '"+path+"', '"+im+"');return false;\" class=\"color\" >"+path+"</a>" 
	var t2 = a.scrollHeight;
	a.scrollTop = tt-(t-t2);
	this.preventDefault();
	return false;
}

function makeimage( e, path, im ){
	var a = document.getElementById("textbox");
	var t = a.scrollHeight;
	
	
	document.getElementById(im).innerHTML = "<a href=\""+path+"\" target=\"_blank\" class=\"color\">"+path+"</a><br/><img src=\""+path+"\" style=\"max-width:100%;height:auto;\" onclick=\"unmakeimage( '"+e+"', '"+path+"','"+im+"');return false;\"/>";
	var t2 = a.scrollHeight;
	a.scrollTop += t2-t;
	this.preventDefault();

	return false;
}

var imgnumber = 1;
function putmessage( room, color, nick, contents ){
	if( rooms[room.toLowerCase()] != null ){
		for( var i = 0; i < users[rooms[room.toLowerCase()]].length; ++i ){
			var cto = users[rooms[room.toLowerCase()]][i];
			
			if("~#+@".indexOf(cto[0])>=0){
				cto = cto.substring(1);
			}
			console.log(cto);
			if( cto.toLowerCase() == nick.toLowerCase() ){
				nick =  users[rooms[room.toLowerCase()]][i];
				break;
			}
		}
	}
	if( rooms[room.toLowerCase()] == null ) return false;
	contents = parseraw(contents, true);
	
	var t = contents.match(/action (.*)/i );
	if( t != null ){
		putmsgstamped( room, "<span id=\"text\" style=\"font-style:italic;\"><span style=\"color:#" + color + ";font-style:italic;cursor:pointer;\" ondblclick=\"startpm('"+nick+"')\">" + nick + " </span> "+ contents.substring(8,contents.length-1)+"</span>" ); 
	}
	else{
		putmsgstamped( room, "<span id=\"text\"><span style=\"color:#" + color + ";cursor:pointer;\" ondblclick=\"startpm('"+nick+"')\">" + nick + "</span> "+ contents+"</span>" ); 
	}
	if(rooms[room.toLowerCase()] != tabactive)
		tabs[rooms[room.toLowerCase()]].setAttribute("class", "inactivehighlight" );
}
function getuserind( rm, usr ){
	var ind = rooms[rm.toLowerCase()];
	for( var i = 0; i < users[ind].length; ++i ){
		var t = users[ind][i];
		if( "+#@~".indexOf(t[0]) > 0 ){
			t = t.substring(1);
		}
		if( t.toLowerCase() == usr.toLowerCase() ) return i;
	}
	return -1;
}

function useraddmode( rm, usr, mod, sender ){
	if( mod == "b" ){
		putmsgstamped( rm, "<span id=\"textcontrol\">"+usr+" was banned by "+sender+"</span>" ); 
	}
	if( mod == "h" ){
		putmsgstamped( rm, "<span id=\"textcontrol\">"+usr+" was given # by "+sender+"</span>" ); 
	}
	if( mod == "o" ){
		putmsgstamped( rm, "<span id=\"textcontrol\">"+usr+" was given @ by "+sender+"</span>" ); 
	}
	if( mod == "v" ){
		putmsgstamped( rm, "<span id=\"textcontrol\">"+usr+" was given + by "+sender+"</span>" ); 
	}
	/*console.log("haha");
	var n = usr;
	var ind = rooms[rm.toLowerCase()];
	var p = getuserind( rm, usr );
	var m = "";
	if( "+#@~".indexOf(users[ind][p][0]) > 0 ){
		m = n[0];
		n = n.substr(1);
	}
	switch( mod ){
		case "v": if( "+#@~".indexOf(m) <= 0 ) m = "+"; break;
		case "h": if( "+#@~".indexOf(m) <= 1 ) m = "#"; break;
		case "o": if( "+#@~".indexOf(m) <= 2 ) m = "@"; break;
		case "a": if( "+#@~".indexOf(m) <= 3 ) m = "~"; break;
		
	}
	users[ind][p][0] = m+n;
	updateuserlist(rm);*/
	Send("WHOIS "+usr+"\r\n");
}
function userremmode( room, usr, mod,sender ){
	if( mod == "b" ){
		putmsgstamped( room, "<span id=\"textcontrol\">"+usr+" was unbanned by "+sender+"</span>" ); 
	}
	if( mod == "h" ){
		putmsgstamped( room, "<span id=\"textcontrol\">"+usr+" had their # revoked by "+sender+"</span>" ); 
	}
	if( mod == "o" ){
		putmsgstamped( room, "<span id=\"textcontrol\">"+usr+" had their @ revoked by "+sender+"</span>" ); 
	}
	if( mod == "v" ){
		putmsgstamped( room, "<span id=\"textcontrol\">"+usr+" had their + revoked by "+sender+"</span>" ); 
	}
	
	
	//console.log("WHOIS "+usr+"\r\n");
	/*var n = usr;
	var ind = rooms[rm.toLowerCase()];
	var p = getuserind( rm, usr );
	var m = "";
	if( "+#@~".indexOf(users[ind][p][0]) > 0 ){
		m = n[0];
		n = n.substr(1);
	}
	switch( mod ){
		case "v": if( "+#@~".indexOf(m) <= 0 ) m = "+"; break;
		case "h": if( "+#@~".indexOf(m) <= 1 ) m = "#"; break;
		case "o": if( "+#@~".indexOf(m) <= 2 ) m = "@"; break;
		case "a": if( "+#@~".indexOf(m) <= 3 ) m = "~"; break;
		
	}
	users[ind][p][0] = m+n;
	updateuserlist(rm);*/
	Send("WHOIS "+usr+"\r\n");
}

function domeow( room ){
	putmsg( room,	 	"<pre>\r\n"+
				"    (\"`-''-/\").___..--''\"`-._\r\n"+
				"     `6_ 6  )   `-.  (     ).`-.__.`)\r\n"+
				"     (_Y_.)'  ._   )  `._ `. ``-..-'\r\n"+
				"   _..`--'_..-_/  /--'_.' ,'\r\n"+
				"  (il).-''  (li).'  ((!.-'\r\n</pre>");
}
function handlemessage(mSG){
	if( mSG == "" || mSG == null ) return false;
	if( mSG.replace(/\s+/g, ' ') == "") return false;
	if( mSG.length < 4 ) return; 
	console.log(mSG);
	var sender = "";
	var message = new Array();
	var args = mSG.split(" ");
	if( mSG[0] == ":" ){
		sender = args[0].substring(1);
		args.splice(0,1);
	}
	for( var i = 0; i < args.length; ++i ){
		if( args[i][0]==":" ){
			args.splice(i);
			args[i] = mSG.substring(mSG.indexOf(" :")+2);
		}
	}
	var nick, user, host;
	nick = user = host = "";
	if( sender != "" ){
		if( sender.indexOf("!") < 0 )
		{
			nick = sender;
		}
		else{
			var t = sender.split("!");
			nick = t[0];
			var u = t[1].split("@");
			user = u[0];
			host = u[1];
		}
	}
	switch(args[0].toLowerCase()){
		case "join": 
			if( nick.toLowerCase() == myname.toLowerCase() ){
				rooms[args[1].toLowerCase()] = tabs.length;
				var newtab = addpmtab(args[1]);

				
				roomcontents[rooms[args[1].toLowerCase()]] = " ";
				roomnames[rooms[args[1].toLowerCase()]] = args[1];
				putmsgstamped( args[1], "<span id=\"textcontrol\">You joined the room.</span>" ); 
				users[rooms[args[1].toLowerCase()]] = new Array();
				Send("WHO "+args[1]+"\r\n" );
				if( loginstate == 1 ){
					loginstate = 0;
					dismisspopup();
				}
				if( rooms[args[1].toLowerCase()] == tabactive ){
					titleshouldbe = roomnames[tabactive] + " - Coldstorm";
					titleshouldbe = "Coldstorm";
					
					document.title = titleshouldbe;
				}
				if( args[1].toLowerCase() != "#2" && args[1].toLowerCase() != "#mod" )
					changetab(newtab); 
			}
			else {
				putmsgstamped( args[1], "<span id=\"textcontrol\"><span style=\"color:#" + user + ";\">" + nick + "</span> joined the room.</span>" ); 
				console.log(user);
				adduser( args[1], nick ); 
				Send("WHOIS "+nick+"\r\n");
				userips[nick.toLowerCase()] = host;
			}
			break;
		case "ping": if( args.length > 1 ) Send("PONG "+args[1]+"\r\n" ); else Send("PONG\r\n" ); break;
		case "notice":
			if( loginstate == 3 ){
				if( nick.toLowerCase() == "nickserv" ){
					if( args[2].toLowerCase() == "access denied." ){
						dopopup("Error","Incorrect password","Retry");
						ws.close();
						document.getElementById("login").style.top = "50%";
						document.getElementById("chat").style.top = "-2000px";
						document.getElementById("lbutt").disabled = false;
						loginstate = -1;
					}
					if( args[2].toLowerCase() == "ghost with your nick has been killed." ){
						loginstate = 2;
						Send("NICK " + toghost + "\r\n");
						Send("PRIVMSG NickServ identify " + document.getElementById("lagpass").value + "\r\n");
						dopopup("Please Wait","Logging In","Dismiss");
					}
					
				}
				
			}
			if( loginstate == 2 ){
				if( nick.toLowerCase() == "nickserv" ){
					if( args[2].toLowerCase() == "password incorrect." ){
						dopopup("Error","Incorrect password","Retry");
						ws.close();
						document.getElementById("login").style.top = "50%";
						document.getElementById("chat").style.top = "-2000px";
						document.getElementById("lbutt").disabled = false;
						loginstate = -1;
					}
					if( args[2].toLowerCase() == "your nick isn't registered." ){
						dopopup("Please Wait","Creating Account","Dismiss");
						loginstate = 4;
						Send("PRIVMSG NickServ register " + document.getElementById("lagpass").value + "\r\n");
					}
					if( args[2].toLowerCase() == "password accepted - you are now recognized." ){
						Send("PRIVMSG Jessica :~fixmyip " + fixtok + "\r\n" );
						dopopup("Please Wait","Joining Rooms","Dismiss");
						loginstate = 10;
												
					}
					
				}
			}
			if( loginstate == 4 ){
				if( nick.toLowerCase() == "nickserv" ){
					if( args[2].toLowerCase().indexOf("registered under your account") >= 0 ){
						dopopup("Account Created","Remember your password:<br/>"+document.getElementById("lagpass").value,"Dismiss");
						Send("PRIVMSG Jessica :~fixmyip " + fixtok + "\r\n" );
						dopopup("Please Wait","Joining Rooms","Dismiss");
						loginstate = 9;
					}
					if( args[2].toLowerCase().indexOf("please try again with a more obscure password.") >= 0 ){
						//dopopup("Account Created","Remember your password:<br/>"+document.getElementById("lagpass"),"Dismiss");
						dopopup("Error","Passwords must be 5 characters long<br/>and must not be easily guessable<br/>such as your real name or nick.","Retry");
						ws.close();
						document.getElementById("login").style.top = "50%";
						document.getElementById("chat").style.top = "-2000px";
						document.getElementById("lbutt").disabled = false;
						loginstate = -1;
						
					}
				}
			}
			if( loginstate == 10 || loginstate == 9 ){
				if( nick.toLowerCase() == "jessica" ){
					if( args[2].toLowerCase() == "tada" ){
						
						loginstate += 9;
						Send("JOIN #Coldstorm\r\n");
						Send("JOIN #2\r\n");
						Send("PRIVMSG Chanserv :unban #mod\r\n");
						
						
						set_cookie("myname",myname);
					}
					if( args[2].toLowerCase() == "invalid token" ){
						dopopup("Error","Unable to verify IP","Retry");
						ws.close();
						document.getElementById("login").style.top = "50%";
						document.getElementById("chat").style.top = "-2000px";
						document.getElementById("lbutt").disabled = false;
						loginstate = -1;
					}
				}
			}
			if( loginstate == 19 || loginstate == 18 ){
				if( nick.toLowerCase() == "chanserv" ){
					if( args[2].toLowerCase().indexOf("you have been unbanned from") == 0 ){
						Send("JOIN #Mod\r\n");
					}
					loginstate -= 18;
					dismisspopup();
					break;
				}
			}
			if( loginstate != 0 ) 
			break;
		case "privmsg": 

			//putmsgstamped( args[1], "<span id=\"text\"><span style=\"color:#" + user + ";\">" + nick + "</span> "+args[2]+"</span>" ); 
			//putmsgstamped( args[1], args[2] ); 

			if( args[1][0] != "#" ){
				if( rooms[nick.toLowerCase()] == null ){
					rooms[nick.toLowerCase()] = tabs.length;
					addpmtab(nick);
					roomcontents[rooms[nick.toLowerCase()]] = " ";
					roomnames[rooms[nick.toLowerCase()]] = nick.toLowerCase();
					users[rooms[nick.toLowerCase()]] = new Array();
				}
				if( args[2].toLowerCase() == "version" ){
					Send("NOTICE "+nick+" :VERSION FrigidStorm - An HTML5 IRC Client written by Kaslai\r\n");
					putmessage( nick, user.substring(0,6), nick, "Recieved a CTCP Version from "+nick );
					break;
				}
				putmessage( nick, user.substring(0,6), nick, args[2] );

			}
			else
			putmessage( args[1], user.substring(0,6), nick, args[2] );
			if( args[2].substring(0,5).toLowerCase() == "~meow" ){
				domeow(args[1]);
			}
			messagesunread += 1;
			var tttt = 0;
			if( document.title[0] == "*" )
			tttt = 1;
			if( tttt == 1 ) tttt = "*** | ["+messagesunread+"] | Coldstorm";
			else tttt = tttt = "["+messagesunread+"] | Coldstorm";
			document.title = tttt;
			break;

		case "part":
			putmsgstamped( args[1], "<span id=\"textcontrol\"><span style=\"color:#" + htmlEntities(user) + ";\">" + nick + "</span> has left.</span>" ); 
			removeuser( args[1], nick );
			if( nick.toLowerCase() == myname.toLowerCase() ) closetab( rooms[args[1].toLowerCase()] );
			break;
		case "quit":
			for( var i = 0; i < roomnames.length; ++i ){
				//if( rooms[roomnames[i]] == null ) continue;
				if( removeuser( roomnames[i], nick ) != false )
					putmsgstamped( roomnames[i], "<span id=\"textcontrol\"><span style=\"color:#" + htmlEntities(user) + ";\">" + nick + "</span> has quit.</span>" ); 
			}
			break;
		case "mode":
			var mode = 1;
			var pos = 0;
			for( var i = 0; i < args[2].length; ++i ){
				if( args[2][i] == "+" ) {mode = 1; continue; }
				if( args[2][i] == "-" ) {mode = 0; continue; }
				if( mode == 1 ) useraddmode( args[1], args[3+pos], args[2][i],nick );
				else userremmode( args[1], args[3+pos], args[2][i],nick );
				pos++;
			}
			break;
		case "nick":
			if( loginstate == 0 ){
				for( var i = 0; i < roomnames.length; ++i ){
					//if( rooms[roomnames[i]] == null ) continue;
					if( removeuser( roomnames[i], nick ) != false ){
						putmsgstamped( roomnames[i], "<span id=\"textcontrol\"><span style=\"color:#" + htmlEntities(user) + ";\">" + nick + "</span> is now known as <span style=\"color:#" + htmlEntities(user) + ";\">" + htmlEntities(args[1]) + "</span></span>" ); 
						adduser(roomnames[i], args[1] );
						colors[args[1].toLowerCase()] = user;
						Send("WHOIS "+args[1]+"\r\n" );
						
					}
				}
			}
			if( nick.toLowerCase() == myname.toLowerCase() ){
				myname = args[1];
			}
			break;
		case "kick":
			if( args[2].toLowerCase() != myname.toLowerCase() ){
				putmsgstamped( args[1], "<span id=\"textcontrol\"><span style=\"color:#" + htmlEntities(colors[args[2].toLowerCase()].substring(0,6)) + ";\">" + args[2] + "</span> has been kicked by <span style=\"color:#" + htmlEntities(user) + ";\">" + nick + "</span>.</span>" ); 
				removeuser( args[1], args[2] );
			}
			else {
				closetab( rooms[args[1].toLowerCase()] );
				putmsgstamped( null, "<span id=\"textcontrol\">You have been kicked from " + htmlEntities(args[1]) + " by <span style=\"color:#" + htmlEntities(user) + ";\">" + nick + "</span>.</span>" ); 
				
			}
			break;
		case "005":
			if( toghost != null ){
				loginstate = 3;
				Send("PRIVMSG NickServ ghost " + toghost + " " + document.getElementById("lagpass").value + "\r\n" );
				dopopup("Please Wait","User already logged in.<br/>Ghosting.","Dismiss");
			}
			else if(document.getElementById("lagpass").value != "") {
				loginstate = 2;
				Send("PRIVMSG NickServ identify " + document.getElementById("lagpass").value + "\r\n");
				dopopup("Please Wait","Logging In","Dismiss");
			}
			else {
				
			}
			//Send("JOIN #Coldstorm\r\n");
			//Send("JOIN #2\r\n");
			//dismisspopup();
			
			break;
		case "332":
			setmotd(args[2], args[3]);
			break;
		case "333":
			motds[args[2].toLowerCase()] += htmlEntities( " (by "+args[3].split("!")[0]+")");
			if( roomnames[tabactive].toLowerCase() == args[2].toLowerCase() )document.getElementById("motd").innerHTML = motds[args[2].toLowerCase()];
			break;
		case "topic":
			setmotd(args[1], args[2] + " (by " + nick + ")");
			
			break;
		
		case "353":
			var r = args[3];
			names=args[4].split(" ");
			for( i = 0; i < names.length; ++i){
				adduser( r, names[i] );
			}
			break;
		case "311":
			colors[args[2].toLowerCase()] = htmlEntities(args[3]);
			console.log( args[2] + "   " + args[3] );
			updateuserlist(tabactive);
			break;
		case "319":
			var t = args[3].split(" ");
			for( var i = 0; i < t.length; ++i ){
				var rodd = t[i];
				var modd = "";
				if( rodd[0] != "#" ){
					modd = rodd[0];
					rodd = rodd.substring(rodd.indexOf("#"));
				}
				if( modd == "%" ) modd = "#";
				rodd=rodd.toLowerCase();
				if( rooms[rodd] != null){
					removeuser( rodd, args[2] );
					adduser(rodd, modd+args[2], 1 );
					if( args[2].toLowerCase() == myname.toLowerCase() )
						myrank[rodd.toLowerCase()] = modd;
					//var q = getuserind( r, nick );
					//users[rooms[r]][q] = m+nick;
					updateuserlist(tabactive);
				}
			}
			break;
			
		case "352":
			colors[args[6].toLowerCase()] = htmlEntities(args[3]);
			userips[args[6].toLowerCase()] = args[4];
			updateuserlist(tabactive);
			break;
		case "367":
			var bl = "<span id=\"textcontrol\">Banned user: "+args[3]+"   Banned by "+args[4];
			if( "%@~".indexOf(myrank[args[2].toLowerCase()]) >= 0 )
				bl += " <a href=\"javascript:void(0);\" class=\"color\" onclick=\"Send('MODE " + args[2] + " -b " + args[3] + "\\r\\n');\">unban</a>";
			bl += "</span>";
			putmsg( args[2], bl );
			break;
		case "433":
			if( toghost == null ) toghost = myname;
			myname += "_";
			Send("NICK "+myname+"\r\n");
			break;
		
			
		
		
	}
}

var sentident = false;


        function msg(str) {
            console.log(str);
        }

        function connect() {
		if( document.getElementById("lagin").value == ""){
			dopopup("Error","Please enter user name","Dismiss");
			return;
		}
		if( document.getElementById("lagpass").value == ""){
			dopopup("Error","Please enter a password","Dismiss");
			return;
		}
		set_cookie("mycolor",document.getElementById("lagcolor").value);
		document.getElementById("lbutt").disabled = true;
		myname = document.getElementById("lagin").value;
		mycolor = document.getElementById("lagcolor").value+mycountry;
		document.getElementById("login").style.top = "-2000px";
		document.getElementById("chat").style.top = "5%";
		dopopup( "Connecting", "Please Wait", "Dismiss" );
		
		
		
		
            var uri = "http://coldstorm.tk:6666";
            ws = new Connection();
            msg("connecting to: " + uri);
            ws.connect(uri,
		function () {
		var msgs = ws.rQshiftStr().split("\r\n");
		for( var i = 0; i < msgs.length; ++i ){
			handlemessage(decodeURIComponent(escape(msgs[i])));
		}
		//msgs.forEach(handlemessage);
		//if( sentident == false )
		    
            },
	    function(){},
		function () {
		Send("QUIT :");
		document.getElementById("login").style.top = "50%";
		document.getElementById("chat").style.top = "-2000px";
		document.getElementById("lbutt").disabled = false;
		    loginstate = -1;
                disconnect();
		for( var i = 0; i < tabs.length; ++i ){
			if( tabs[i] != null ){
				tabs[i].parentNode.removeChild(tabs[i]);
			}
		}
                msg("Disconnected");
		if( loginstate == 0 ){
			dopopup("Error", "Lost connection to the server", "Dismiss");
		}
            }
	    );

            ws.on('message', );
            ws.on('close', );
            ws.on('open', function () {
                msg("Connected");
		Send("NICK "+myname+"\r\n"+"USER " + mycolor + " Coldstormer2 Coldstormer3 :Coldstormer4\r\n");
		colors[myname.toLowerCase()]=mycolor;
		//ws.flush();
            });
	    
	    
        }

	function parsefunc( input ){
		var args = input.split(" ");
		switch( args[0].toLowerCase() ){
			case "ban": case "kickban":
				Send("MODE " + roomnames[tabactive] + " +b " + args[1] + "!*@*\r\n");
				/*for( var i = 0; i < roomnames.length; ++i ){
					if( roomnames[i] != null ){
						var tosend = "PRIVMSG ChanServ :BAN " + roomnames[i];
						for( var j = 1; j < args.length; j++ )
							tosend += " " + args[j];
						Send( tosend + "\r\n" );
					}
				}*/
				if( args[0].toLowerCase() == "ban" );
				break;
			case "kick": 
				Send("KICK " + roomnames[tabactive] + " " + args[1] + "\r\n");
				/*for( var i = 0; i < roomnames.length; ++i ){
					if( tabs[i] != null ){
						var tosend = "PRIVMSG ChanServ :KICK " + roomnames[i];
						for( var j = 1; j < args.length; j++ )
							tosend += " " + args[j];
						Send( tosend + "\r\n" );
					}
				}*/
				break;
			case "unban":
				Send("MODE " + roomnames[tabactive] + " -b " + args[1] + "!*@*\r\n");
				break;
			case "kickbanip":
			case "banip":
				Send("MODE " + roomnames[tabactive] + " +b *!*@"+userips[args[1].toLowerCase()]+"\r\n");
				if( args[0].toLowerCase() == "banip" );
				break;
				Send("KICK " + roomnames[tabactive] + " " + args[1] + "\r\n");
				break;
			case "banlist":
				Send("MODE " + roomnames[tabactive] + " +br\n");
				break;
			case "me":
				var ts ="ACTION " + input.substring(3) + "\r\n";
				Send( "PRIVMSG " + roomnames[tabactive] + " :" + ts);
				putmessage( roomnames[tabactive], mycolor.substring(0,6), myname, ts );
				break;
			case "pm": case "query": case "msg":
				{
					if( args[1] == "" ) break;
					var nick = args[1];
					args.splice(0,2);
					var s = args.join(" ");
					if( rooms[nick.toLowerCase()] == null ){
						rooms[nick.toLowerCase()] = tabs.length;
						addpmtab(nick);
						roomcontents[rooms[nick.toLowerCase()]] = " ";
						roomnames[rooms[nick.toLowerCase()]] = nick.toLowerCase();
						users[rooms[nick.toLowerCase()]] = new Array();
					}
					putmessage( nick, mycolor, myname, s );
					Send("PRIVMSG "+nick+" :" + s + "\r\n" );
				}
				break;
			case "voice": case "vop": case "+":
				switch( roomnames[tabactive].toLowerCase() ){
					case "#coldstorm": case "#2": case "#mod": 
						Send(  "PRIVMSG ChanServ :ACCESS #Coldstorm ADD "+ args[1]+" 1\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #2 ADD "+ args[1]+" 1\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #Mod ADD "+ args[1]+" 1\r\n" );
						Send(  "MODE #Coldstorm +v "+ args[1]+"\r\n" );
						Send(  "MODE #2 +v "+ args[1]+"\r\n" );
						Send(  "MODE #mod +v "+ args[1]+"\r\n" );
					break;
					default: Send(  "MODE " + roomnames[tabactive] + " +v "+ args[1]+"\r\n" );//Send(  "PRIVMSG ChanServ :ACCESS " + roomnames[tabactive] + " ADD "+ args[1]+" 1\r\n" );
				}
				break;
			case "halfop": case "hop": case "#":
				switch( roomnames[tabactive].toLowerCase() ){
					case "#coldstorm": case "#2": case "#mod": 
						Send(  "PRIVMSG ChanServ :ACCESS #Coldstorm ADD "+ args[1]+" 2\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #2 ADD "+ args[1]+" 2\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #Mod ADD "+ args[1]+" 2\r\n" );
						Send(  "MODE #Coldstorm +h "+ args[1]+"\r\n" );
						Send(  "MODE #2 +h "+ args[1]+"\r\n" );
						Send(  "MODE #mod +h "+ args[1]+"\r\n" );
					break;
					default: Send(  "MODE " + roomnames[tabactive] + " +h "+ args[1]+"\r\n" );//Send(  "PRIVMSG ChanServ :ACCESS " + roomnames[tabactive] + " ADD "+ args[1]+" 1\r\n" );
				}
				break;
			case "operator": case "op": case "@":
				switch( roomnames[tabactive].toLowerCase() ){
					case "#coldstorm": case "#2": case "#mod": 
						Send(  "PRIVMSG ChanServ :ACCESS #Coldstorm ADD "+ args[1]+" 3\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #2 ADD "+ args[1]+" 3\r\n" );
						Send(  "PRIVMSG ChanServ :ACCESS #Mod ADD "+ args[1]+" 3\r\n" );
						Send(  "MODE #Coldstorm +o "+ args[1]+"\r\n" );
						Send(  "MODE #2 +o "+ args[1]+"\r\n" );
						Send(  "MODE #mod +o "+ args[1]+"\r\n" );
					break;
					default: Send(  "MODE " + roomnames[tabactive] + " +v "+ args[1]+"\r\n" );//Send(  "PRIVMSG ChanServ :ACCESS " + roomnames[tabactive] + " ADD "+ args[1]+" 1\r\n" );
				}
				break;
			case "mute":
				muted = true;
				set_cookie("muted","true");
				putmsgstamped(  roomnames[tabactive], "<span id=\"textcontrol\">Sounds have been muted.</span>" ); 
				break;
			case "unmute":
				muted = false;
				set_cookie("muted","false");
				putmsgstamped(  roomnames[tabactive], "<span id=\"textcontrol\">Sounds have been unmuted.</span>" ); 
				break;
			case "join":
				Send( input + "\r\n" );
				break;
			case "clear":
				roomcontents[tabactive] = "";
				changetab(tabactive); 
				break;
			case "meow":
				Send( "PRIVMSG " + roomnames[tabactive] + " :~meow\r\n" );
				putmessage( roomnames[tabactive], mycolor.substring(0,6), myname, "~meow" );
				domeow( roomnames[tabactive] );
				break;
			case "motd":
				Send( "TOPIC " + roomnames[tabactive] + " :" + input.substring(input.indexOf(" ")+1) + "\r\n" );
				break;
			case "help":
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">Current commands are:</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\"></span>" );
				if( "#@~".indexOf(myrank[roomnames[tabactive].toLowerCase()]) >= 0 ){
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/kick [name]</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/ban [name]</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/kickban [name]</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/banip [name]</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/kickbanip [name]</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/banlist</span>" );
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/motd [motd]</span>" );
					
				}
				if( "#@~".indexOf(myrank[roomnames[tabactive].toLowerCase()]) >= 0 )
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/voice OR /vop OR /+</span>" );
				if( "@~".indexOf(myrank[roomnames[tabactive].toLowerCase()]) >= 0 )
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/halfop OR /hop OR /#</span>" );
				if( "@~".indexOf(myrank[roomnames[tabactive].toLowerCase()]) >= 0 )
					putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/operator OR /op OR /@</span>" );
				
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/mute and /unmute to mute/unmute the ping sound.</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/join to join other rooms.</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/clear to clear the chat log in the current tab.</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/meow</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\">/help</span>" );
				putmsg( roomnames[tabactive], "<span id=\"textcontrol\"></span>" );
				
			default:
				//Send( input + "\r\n" );
			
		}
	}
	
        function disconnect() {
            if (ws) { ws.close(); }
            ws = null;
        }

/*
** Returns the caret (cursor) position of the specified text field.
** Return value range is 0-oField.value.length.
*/
function doGetCaretPosition (oField) {

  // Initialize
  var iCaretPos = 0;

  // IE Support
  if (document.selection) {

    // Set focus on the element
    oField.focus ();

    // To get cursor position, get empty selection range
    var oSel = document.selection.createRange ();

    // Move selection start to 0 position
    oSel.moveStart ('character', -oField.value.length);

    // The caret position is selection length
    iCaretPos = oSel.text.length;
  }

  // Firefox support
  else if (oField.selectionStart || oField.selectionStart == '0')
    iCaretPos = oField.selectionStart;

  // Return results
  return (iCaretPos);
}
function setCaretPosition(ctrl, pos)
{

    if(ctrl.setSelectionRange)
    {
        ctrl.focus();
        ctrl.setSelectionRange(pos,pos);
    }
    else if (ctrl.createTextRange) {
        var range = ctrl.createTextRange();
        range.collapse(true);
        range.moveEnd('character', pos);
        range.moveStart('character', pos);
        range.select();
    }
}
var tabprev = -1;
var tabprevv = "";
var tabprevp = 0;

document.getElementById("querybox").onkeydown=function(e){
	console.log( e.keyCode );
	if( e.keyCode == 13 ){
		var snd =  this.value;
		snd = snd.replace(/\\c/gi, "" );
		snd = snd.replace(/\\r/gi, "" );
		snd = snd.replace(/\\b/gi, "" );
		
		
		if( snd[0] == "/" ){
			if( snd[1] == "/" ){
				Send( "PRIVMSG " + roomnames[tabactive] + " :" + snd.substring(1) + "\r\n" );
				putmessage( roomnames[tabactive], mycolor.substring(0,6), myname, snd.substring(1) );
			}
			else
			{
				//Send( this.value.substring(1) + "\r\n" );
				parsefunc( snd.substring(1) );
			}
		}
		else{
			if( snd == "" ) return;
			Send( "PRIVMSG " + roomnames[tabactive] + " :" + snd + "\r\n" );
			putmessage( roomnames[tabactive], mycolor.substring(0,6), myname, snd );
		}
		
		//putmessage( roomnames[tabactive], mycolor, myname, this.value );
		
		this.value = "";
	}
	if( e.keyCode == 9 ){
		this.focus();
		var p = doGetCaretPosition(this);
		var i;
		var v = "";
		i = 0;
		if( tabprev == -1 ){
			if( tabprev > -1 ){
				i--;
			}
			for( i+=p-1; this.value.substring(i,i+1).match(/[^\[\]\w\d_`-{}]/i)==null && i >= 0; --i ){
				
			}
			v = this.value.substring(i+1,p);
			tabprevp = i;
		}
		i = tabprevp;
		var v2 = v;
		if( tabprev == -1 )
			tabprevv = v;
		else
			v = tabprevv;
		var ii = i;
		var sett = 0;
		if( tabprev == -3 ) tabprev = -1;
		for( i = tabprev+1; i < users[tabactive].length; ++i ){
			var cto = users[tabactive][i];
			if( "+#@~".indexOf(cto[0])>=0 )
				cto = cto.substring(1);
			if( cto.toLowerCase().indexOf(v.toLowerCase())==0 ){
				tabprev = i;
				sett = 1;
				v2 = users[tabactive][i]; 
				if( "+#@~".indexOf(v2[0])>=0 )
				v2 = v2.substring(1);
				break;
			}
		}
		if( sett == 0 ) {tabprev = -3; v2 = tabprevv;}
		this.value = this.value.substring(0,ii+1)+v2+" "+this.value.substring(p);
		setCaretPosition(this, ii+v2.length+2);
		console.log(v2);
			if(e.preventDefault) {
                e.preventDefault();
            }
            return false;
	}
	else tabprev = -1;

}

window.onbeforeunload = function() {
if(loginstate == -1) return;
  var message = "Are you sure you wish to leave " + titleshouldbe+"?",
  e = e || window.event;
  // For IE and Firefox
  if (e) {
    e.returnValue = message;
  }

  // For Safari
  return message;
    //return confirm("Are you sure you wish to leave " + titleshouldbe+"?" );
};

//document.getElementById("querybox").focus();
document.getElementById("lbutt").onclick = function(){connect();}
document.getElementById("lbutt").disabled = false;
document.getElementById("dbutt").onclick = function(){dismisspopup();}
document.getElementById("dbutt").disabled = true;
document.getElementById("lagin").onkeypress=function(e){
	if( e.keyCode == 13 ){
		document.getElementById("lagpass").focus();
	}
}
document.getElementById("lagpass").onkeypress=function(e){
	if( e.keyCode == 13 ){
		connect();
	}
}
if( get_cookie("donepopup") == null ){
	dopopup("Notice","Accounts have been implemented.<br/>Just type in your name and<br/>desired password. Your account<br/>will be created automatically","Dismiss");
	set_cookie( "donepopup", "true" );
}
if( get_cookie("mycolor") != null ){
	document.getElementById("lagcolor").value = get_cookie("mycolor");
}
if( get_cookie("myname") != null ){
	document.getElementById("lagin").value = get_cookie("myname");
}
if( get_cookie("muted") != null ){
	muted = get_cookie( "muted" )=="true"?true:false;
}
window.onblur=function() { document.title = titleshouldbe; messagesunread=0;}
window.onclick=function() { document.title = titleshouldbe; messagesunread=0;}
window.onkeyup=function() { document.title = titleshouldbe; messagesunread=0;}

window.onkeyup=function() { /*if( document.getElementById("chat").style.top == "50%" ) document.getElementById("querybox").focus();*/}

document.getElementById("lbutt").disabled = false;