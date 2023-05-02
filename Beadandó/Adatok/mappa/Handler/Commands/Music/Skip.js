const error = require('../../../Response/Error');

module.exports = {
	name:'skip',
	category:'Service',
	description:'$skip (Jelenlegi szám átugrása)',
	isload: true,
	run: async (bot, msg, args, guildqueue) => {
		if (args.length === 0) {
			if (msg.guild.members.me.voice.channel !== null || msg.member.voice.channel !== null) {
				if (msg.member.voice.channel.id === msg.guild.members.me.voice.channel.id) {
					if (guildqueue !== undefined && guildqueue !== null) {
						guildqueue.skip();
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