const fs = require('fs');
const { EmbedBuilder } = require('discord.js');

module.exports = {
	name:'help',
	category:'root_service',
	description:'$help',
	isload:true,
	run: async (bot, msg) => {
		const com = fs.readdirSync('./Handler/Commands/Music/').filter(file => file.endsWith('.js'));

		const content = new EmbedBuilder()
			.setTitle('$help command parancs leírás: ')
			.setColor('#00FF00')
			.setTimestamp();

		for (const elements of com) {
			const pull = require(`../Music/${elements}`);
			content.addFields({ name: 'Így használd: ', value: pull.description, inline: true });
		}

		msg.channel.send({ embeds: [content] });
	},
};