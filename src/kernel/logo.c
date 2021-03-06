/*
	This file is part of an x86_64 hobbyist operating system called KnutOS
	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/

	Copyright (C) 2021-2022  Yves Vollmeier <https://github.com/Tix3Dev>
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

const char *small_logo_text =	"┌┐┌─┐         ┌┐ ┌───┐┌───┐\n"
                                "│││┌┘        ┌┘└┐│┌─┐││┌─┐│\n"
                                "│└┘┘ ┌─┐ ┌┐┌┐└┐┌┘││ │││└──┐\n"
                                "│┌┐│ │┌┐┐││││ ││ ││ ││└──┐│\n"
                                "│││└┐│││││└┘│ │└┐│└─┘││└─┘│\n"
                                "└┘└─┘└┘└┘└──┘ └─┘└───┘└───┘\n";

const char *big_logo	    =	"MMMMMMMMMMMMMMMMMMMMMMMNKkdoooloolloox0WMMMMMMMMMMMMMMMMMMMXkl;'..    ..':oxKWMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMWKd::lc;'...'..,:c;lKMMMMMMMMMMMMMMMMXd,.            'lc;cOWMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMNd;:kXk,         .co;:0NXXK00O000KKKXk;;:.            .kWKl,oNMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMXl,xNMNc            ,:.'clloooooooooll,.:c.            .xMMNd'oNMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMWd'xWMMWk'           .:oOXNWMMMMMMMMMWNXOdl;'          .oXMMMNo'xWMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMXc'dkxxddc.    .. 'oOXWMMMMMMMMMMMMMMMMMMMMW0,;xxoc;'..;x0NMMMK;cNMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMN0xlc;:looddxxo,.'o0d'dWMMMMMMMMMMMMMMMMMMMMMMMMO,oWMMMWN0:.':cld0k,lNMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMWXkl:cld0NWMMMMNOl::xXMMO,dWMMMMMMMMMMMMMMMMMMMMMMMM0;oWMMMMMMK:,x0kl:'.lXWMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMW0l:cd0NMMMMMWXkl::o0NMMMM0,oWMMMMMMMMMMMMMMMMMMMMMMMMK;cNMMMMMMMKc,xNMNOoc:cdOXWMMMMMMMMMMMM\n"
                                "MMMMMW0l;l0WMMMMMWKxl:cd0NMMMMMMM0,lWMMMMMMMMMMMMMMMMMMMMMMMMNl,OMMMMMMMMNo,lKMMMWKkocccxXMMMMMMMMMM\n"
                                "MMMWKo;c0WMMMWKkoc:lxKWMMMMMMMMMM0,lWMMMMMMMMMMMMMMMMMMMMMMMMMK;;0MMMMMMMMWO:;xNMMMMMWXx:;dXMMMMMMMM\n"
                                "MMKo;cOWMMMXxc:cdONMMMMMMMMMMMMMM0,lWMMMMMMMMMMMMMMMMMMMMMMMMMWO,cXMMMMMMMMMXd;:OWMMMMMMNk:;kNMMMMMM\n"
                                "MK:;kWMMMXd;:xXWMMMMMMMMMMMMMMMMM0,lWMMMMMMMMMMMMMMMMMMMMMMMMMMWd'xWMMMMMMMMMWKl,oXMMMMMMMXo,lKWMMMM\n"
                                "Nl,OMMMW0:;xNMMMMMMMMMMMMMMMMMMMMO,oWMMMMMMMMMMMMMMMMMMMMMMMMMMMX:;KMMMMMMMMMMMWk,cKMMMMMMMW0:;xNMMM\n"
                                "O,oWMMWk,:0WMMMMMMMMMMMMMMMMMMMMWd,kMMMMMMMMMMMMMMMMMMMMMMMMMMMMMk,oWMMMMMMMMMMMWO,cXMMMMMMMMXo,lXMM\n"
                                "o,OMMWx,lXMMMMMMMMMMMMMMMMMMMMMM0;cXMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNc;0MMMMMMMMMMMMWx,oNMMMMMMMWNx,lNM\n"
                                ";:XMMO,cNMMMMMMMMMMMMMMMMMMMMMMNl,OMM┌┐┌─┐MMMMMMMMM┌┐M┌───┐┌───┐MMk,dWMMMMMMMMMMMMNl,OMMMMMMNddXo,OM\n"
                                ",oWMNl,0MMMMMMMMMMMMMMMMMMMMMMMk,oWMM│││┌┘MMMMMMMM┌┘└┐│┌─┐││┌─┐│MMX::XMMMMMMMMMMMMMO,oWXKNMMK;cNk,dW\n"
                                ",oXXK::XMMMMMMMMMMMMMMMMMMMMMMX:;KMMM│└┘┘M┌─┐M┌┐┌┐└┐┌┘││M│││└──┐MMWo,OMMMMMMMMMMMMMK;,o;lXW0:,OMO,oW\n"
                                ";cx::,:XMMMMMMMMMMMMMMMMMMMMMMk,dWMMM│┌┐│M│┌┐┐││││M││ ││M││└──┐│MMMO,oWMMMMMMMMMMMMX;.,dXWO,:0WMX:;K\n"
                                "::0x' .dKNMMMMMMMMMMMMMMMMMMMNc,0MMMM│││└┐│││││└┘│M│└┐│└─┘││└─┘│MMMX::XMMMMMMMMMMMNo,oXMMK;;KWNNXx'o\n"
                                "l,do,,clcclkNMMMMMMMMMMMMMMMM0,lNMMMM└┘└─┘└┘└┘└──┘M└─┘└───┘└───┘MMMWd,kMMMMMMMMMMNo'oXWNx,.:lcloll,;\n"
                                "d'lxOXMMW0o::dXMMMMMMMMMMMMMMx,xMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMK;:XMMMMMMNklc;,cllc'';,.,oONO;:\n"
                                "0;cXMMMMMMMXx:;xNMMMMMMMMMMMWo,OMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWd,kMMMMMXc,oKNNNNXXXXNN0xd0No,x\n"
                                "Wx'oNMMMMMMMMNd,c0WMMMMMMMMMNc;KMMMMMMMW0ONMMMMMMMMMMMMWK0NMMMMMMMMMMk,dWMMMXc,kWMMMMMMMMMMMMMMNd,lN\n"
                                "MNd'lXMMMMMMMMW0c,dXMMMMMMMMO,lWMMMMMMMNc;KMMMMMMMMMMMMNl;KMMMMMMMMMMx,xMMMNl,kWMMMMMMMMMMMMMNk:;dNM\n"
                                "MMWk,:KMMMMMMMMMNk:;xXMMMMMWd,kMMMMMMMMNl:KMMMMMMMMMMMMWd'xWMMMMMMMMK::KMW0c,xWMMMMMMMMMMMMXd::dKWMM\n"
                                "MMMW0;;0MMMMMMMMMMNx:;oKWMMWo;0MMMMMMMMWK0WMMMMMMMMMMMMMXxOWMMMMMMMMd,xXkc;l0WMMMMMMMMMMMWk;;kNMMMMM\n"
                                "MMMMMK:;OWMMMMMMMMMMNOl;cdO0c;KMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNO;.;::dKWMMMMMMMMMMMMNo'lXMMMMMMM\n"
                                "MMMMMMKc,xNMMMMMMMMMMMWXkolc''lddxO0KXXNNWWMMMMMMMMMMMMWWNNX0kdoll:. .ckNMMMMMMMMMMMMMW0c,dNMMMMMMMM\n"
                                "MMMMMMMXd,c0WMMMMMMMMMMMMMWNKkd:..clc,..',;:ccccccc:::;;,'...,oxkd;,oKWMMMMMMMMMMMMMMNd,c0WMMMMMMMMM\n"
                                "MMMMMMMMW0c,oXMMMMMMMMMMMMMMMMMNd;:dko.                    .cKWKl,l0WMMMMMMMMMMMMMMWO:;xNMMMMMMMMMMM\n"
                                "MMMMMMMMMMWk:;xXMMMMMMMMMMMMMMMMMXkl;:;'..''............';lxOxl;cOWMMMMMMMMMMMMMMMXl,lKMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMNx:;dXWMMMMMMMMMMMMMMMMWXkollllcccccccc;.,cclllllld0WMMMMMMMMMMMMMMMNx;:OWMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMNkc;oKWMMMMMMMMMMMMMMMMMMMWWNNNNNNNNNd'dNWWWWWMMMMMMMMMMMMMMMMMMXx:;xXMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMWOc;l0WMMMMMMMMMMMMMMMMMMMMMMMMMMWNl,OMMMMMMMMMMMMMMMMMMMMMWKo;:xXMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMW0l;lONMMMMMMMMMMMMMMMMMMMMMMMMKx;cXMMMMMMMMMMMMMMMMMMMNkc;ckNMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMW0o::dKWMMMMMMMMMMMMMMMMMMMMMx',xMMMMMMMMMMMMMMMMMWKd::dKWMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMXkc:coxKNMMMMMMMMMMMMMMMMNc ;KMMMMMMMMMMMMMMMNkc;ckNMMMMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMMMWXOdl:clxKNMMMMMMMMMMMMX: cNMMMMMMMMMMMWNOo::dKWMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMWN0xlcclox0XWMMMMMMK;.lNMMMMMMMMN0dlcclONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWNKOdocccldOKNWX: :XMWWNXOdlc:lxKWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWX0xolllll,.'loollllox0NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n"
                                "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWX0OkkkkOO0KNWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n";
