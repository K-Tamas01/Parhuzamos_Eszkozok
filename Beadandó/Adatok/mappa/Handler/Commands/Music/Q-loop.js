const error = require('../../../Response/Error');

module.exports = {
	name:'q-loop',
	category:'Service',
	description:'$q-loop (lejátszási lista loop be/ki -kapcsolása)',
	isload: true,
	run: async (bot, msg, args, guildqueue) => {
		if (args.length === 0) {
			if (msg.guild.members.me.voice.channel !== null || msg.member.voice.channel !== null) {
				if (msg.member.voice.channel.id === msg.guild.members.me.voice.channel.id) {
					if (guildqueue !== undefined && guildqueue !== null) {
						if (guildqueue.repeatMode === 0) {
							guildqueue.setRepeatMode(2);
						}
						else if (guildqueue.repeatMode === 2) {
							guildqueue.setRepeatMode(0);
						}
						else if (guildqueue.repeatMode === 1) {
							error.run(msg, 6, guildqueue, args);
						}
					}
					else {
						error.run(msg, 4, guildqueue, args);
					}
				}
				else {
					error.run(msg, 3, guildqueue, args);
				}
			}
			else {
				error.run(msg, 2, guildqueue, args);
			}
		}
		else {
			error.run(msg, 1, guildqueue, args);
		}
	},
};