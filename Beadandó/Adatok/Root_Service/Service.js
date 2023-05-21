module.exports = { isnum, range, isplaylist };


function isplaylist(link) {
	const isplay = link.toString().split('/').join().split('?').join().split('&').join().split('=').join().split(',');
	if (isplay[0] === 'https:') {
		if (isplay[3] !== 'playlist' && isplay[3] !== 'album' && isplay[6] !== 'list') {return true;}
		else {return false;}
	}
	else {return true;}
}

function isnum(args) {
	if (isNaN(parseInt(args))) return false;
	else return true;
}

function range(length, args) {
	if (length < parseInt(args) || parseInt(args) == 0) return false;
	return true;
}