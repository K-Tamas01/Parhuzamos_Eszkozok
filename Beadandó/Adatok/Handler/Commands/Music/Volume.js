module.exports = {
	name:'volume',
	description:'hangerő csökkentés és növelés',
	isload:true,
	run:async (guildqueue, vol) => {
		guildqueue.setVolume(vol);
	},
};